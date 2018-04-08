#include <algorithm>
#include <core/sleep.hh>
#include <CQLDriver/Common/Exceptions/NotImplementedException.hpp>
#include <CQLDriver/Common/Exceptions/ConnectionNotAvailableException.hpp>
#include <CQLDriver/Common/Exceptions/LogicException.hpp>
#include <CQLDriver/Common/NodeCollection.hpp>
#include <CQLDriver/Common/SessionConfiguration.hpp>
#include "./ConnectionPool.hpp"
#include "./Connection.hpp"

namespace cql {
	/** Try to get a connection with idle stream, may return (nullptr, {}) if not available */
	std::pair<seastar::lw_shared_ptr<Connection>, ConnectionStream>
		ConnectionPool::tryGetConnection() {
		// find connection which has most free streams
		decltype(allConnections_)::iterator it;
		while (true) {
			it = std::max_element(allConnections_.begin(), allConnections_.end(), [](auto& a, auto& b) {
				return a->getFreeStreamsCount() < b->getFreeStreamsCount();
			});
			if (it == allConnections_.end()) {
				// no connection available
				return { nullptr, ConnectionStream() };
			} else if ((*it)->isReady()) {
				// found a usable conection
				break;
			} else {
				// found a closed connection
				allConnections_.erase(it);
			}
		}
		// get stream from the connection
		auto stream = (*it)->openStream();
		if (!stream.isValid()) {
			// max free stream count is 0
			return { nullptr, ConnectionStream() };
		}
		return { *it, std::move(stream) };
	}

	/** Get a connection with idle stream, wait until they are available */
	seastar::future<seastar::lw_shared_ptr<Connection>, ConnectionStream>
		ConnectionPool::getConnection() {
		// always create new connection until min pool size is reached
		// call this function concurrently may creates more connections than min pool size,
		// but it's acceptable for now
		if (allConnections_.size() < sessionConfiguration_->getMinPoolSize()) {
			return makeConnection();
		}
		// use existing connections
		auto result = tryGetConnection();
		if (result.first.get() != nullptr) {
			return seastar::make_ready_future<
				seastar::lw_shared_ptr<Connection>, ConnectionStream>(
				result.first, std::move(result.second));
		}
		// create new connection if all existing connections is occupied until max pool size is reached
		if (allConnections_.size() < sessionConfiguration_->getMaxPoolSize()) {
			dropIdleConnectionTimer();
			return makeConnection();
		}
		// wait until some connection is available
		seastar::promise<seastar::lw_shared_ptr<Connection>, ConnectionStream> promise;
		auto future = promise.get_future();
		if (!waiters_.push(std::move(promise))) {
			return seastar::make_exception_future<
				seastar::lw_shared_ptr<Connection>, ConnectionStream>(
				ConnectionNotAvailableException(CQL_CODEINFO, "no connections available"));
		} else {
			dropIdleConnectionTimer();
			findIdleConnectionTimer();
		}
		return future;
	}

	/** Return the connection to the pool manually, this is optional */
	void ConnectionPool::returnConnection(
		seastar::lw_shared_ptr<Connection>&& connection, ConnectionStream&& stream) {
		stream.close();
		if (!waiters_.empty()) {
			auto promise = waiters_.pop();
			auto stream = connection->openStream();
			if (!stream.isValid()) {
				promise.set_exception(LogicException(CQL_CODEINFO,
					"open stream form a returned connection failed"));
			} else {
				promise.set_value(std::move(connection), std::move(stream));
			}
		}
	}

	/** Constructor */
	ConnectionPool::ConnectionPool(
		const seastar::lw_shared_ptr<SessionConfiguration>& sessionConfiguration,
		const seastar::shared_ptr<NodeCollection>& nodeCollection) :
		sessionConfiguration_(sessionConfiguration),
		nodeCollection_(nodeCollection),
		allConnections_(),
		waiters_(sessionConfiguration_->getMaxWaitersAfterConnectionsExhausted()),
		findIdleConnectionTimerIsRunning_(false),
		dropIdleConnectionTimerIsRunning_(false) { }

	/** Make a new ready-to-use connection and return it with an idle stream */
	seastar::future<seastar::lw_shared_ptr<Connection>, ConnectionStream>
		ConnectionPool::makeConnection() {
		seastar::promise<seastar::lw_shared_ptr<Connection>, ConnectionStream> promise;
		auto future = promise.get_future();
		auto self = shared_from_this();
		seastar::do_with(
			std::move(promise), std::move(self), static_cast<std::size_t>(0U),
			[] (auto& promise, auto& self, auto& count) {
			return seastar::repeat([&promise, &self, &count] {
				// choose one node and create connection
				auto node = self->nodeCollection_->chooseOneNode();
				auto connection = seastar::make_lw_shared<Connection>(
					self->sessionConfiguration_, node);
				// initialize connection
				return connection->ready().then([&promise, &self, node, connection] {
					// initialize connection success
					auto stream = connection->openStream();
					if (!stream.isValid()) {
						promise.set_exception(LogicException(CQL_CODEINFO,
							"open stream form a newly created connection failed"));
					} else {
						self->nodeCollection_->reportSuccess(node);
						self->allConnections_.emplace_back(connection);
						promise.set_value(std::move(connection), std::move(stream));
					}
					return seastar::stop_iteration::yes;
				}).handle_exception([&promise, &self, &count, node] (std::exception_ptr ex) {
					// initialize connection failed, try next node until all tried
					self->nodeCollection_->reportFailure(node);
					if (++count >= self->nodeCollection_->getNodesCount()) {
						promise.set_exception(ex);
						return seastar::stop_iteration::yes;
					} else {
						return seastar::stop_iteration::no;
					}
				});
			});
		});
		return future;
	}

	/** Timer used to find idle connection and feed waiters */
	void ConnectionPool::findIdleConnectionTimer() {
		static const std::size_t SleepInterval = 1;
		if (findIdleConnectionTimerIsRunning_) {
			return;
		}
		findIdleConnectionTimerIsRunning_ = true;
		auto self = shared_from_this();
		seastar::do_with(std::move(self), [] (auto& self) {
			return seastar::repeat([&self] {
				return seastar::sleep(std::chrono::milliseconds(SleepInterval)).then([&self] {
					// feed waiters
					while (!self->waiters_.empty()) {
						auto result = self->tryGetConnection();
						if (result.first.get() != nullptr) {
							self->waiters_.pop().set_value(
								std::move(result.first), std::move(result.second));
						} else {
							break;
						}
					}
					// stop timer if no waiters
					return self->waiters_.empty() ?
						seastar::stop_iteration::yes :
						seastar::stop_iteration::no;
				});
			}).finally([&self] {
				self->findIdleConnectionTimerIsRunning_ = false;
				if (!self->waiters_.empty()) {
					self->findIdleConnectionTimer(); // avoid task race
				}
			});
		});
	}

	/** Timer used to drop idle connections */
	void ConnectionPool::dropIdleConnectionTimer() {
		// NOTICE:
		// this may cause all live connections point to the same node for a while,
		// someday I should find a better mechanism to do this.
		static const std::size_t SleepInterval = 500;
		if (dropIdleConnectionTimerIsRunning_) {
			return;
		}
		dropIdleConnectionTimerIsRunning_ = true;
		auto self = shared_from_this();
		seastar::do_with(std::move(self), [] (auto& self) {
			return seastar::repeat([&self] {
				return seastar::sleep(std::chrono::milliseconds(SleepInterval)).then([&self] {
					auto existCount = self->allConnections_.size();
					auto minCount = self->sessionConfiguration_->getMinPoolSize();
					if (existCount <= minCount) {
						// can't drop any connection more
						return seastar::stop_iteration::yes;
					}
					self->allConnections_.erase(std::remove_if(
						self->allConnections_.begin(),
						self->allConnections_.end(),
						[&existCount, &minCount] (const auto& connection) {
							// drop closed or redundant connections
							if (!connection->isReady() ||
								(existCount > minCount && connection->isAllStreamsFree())) {
								--existCount;
								return true;
							}
							return false;
						}),
						self->allConnections_.end());
					// stop timer if can't drop any connection more
					return existCount <= minCount ?
						seastar::stop_iteration::yes :
						seastar::stop_iteration::no;
				});
			}).finally([&self] {
				self->dropIdleConnectionTimerIsRunning_ = false;
			});
		});
	}
}

