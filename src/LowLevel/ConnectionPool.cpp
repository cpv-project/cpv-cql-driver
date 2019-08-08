#include <algorithm>
#include <seastar/core/sleep.hh>
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
				metricsData_->pool_connections_current = allConnections_.size();
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
		// always spawn connection until min pool size is reached
		auto minPoolSize = sessionConfiguration_->getMinPoolSize();
		if (allConnections_.size() < minPoolSize) {
			auto future = addWaiter();
			if (allConnections_.size() + connectingCount_ < minPoolSize) {
				spawnConnection();
			} else {
				findIdleConnectionTimer();
			}
			return future;
		}
		// use existing connections
		auto result = tryGetConnection();
		if (result.first.get() != nullptr) {
			return seastar::make_ready_future<
				seastar::lw_shared_ptr<Connection>, ConnectionStream>(
				result.first, std::move(result.second));
		}
		// spawn connection if no connection available until max pool size is reached
		auto maxPoolSize = sessionConfiguration_->getMaxPoolSize();
		auto poolIsFull = allConnections_.size() + connectingCount_ >= maxPoolSize;
		if (CQL_UNLIKELY(poolIsFull && waiters_.size() >=
			sessionConfiguration_->getMaxWaitersAfterConnectionsExhausted())) {
			// can't spawn more connection and can't add more waiter
			return seastar::make_exception_future<
				seastar::lw_shared_ptr<Connection>, ConnectionStream>(
				ConnectionNotAvailableException(CQL_CODEINFO, "no connections available"));
		} else {
			auto future = addWaiter();
			if (!poolIsFull) {
				spawnConnection();
				dropIdleConnectionTimer();
			} else {
				findIdleConnectionTimer();
			}
			return future;
		}
	}

	/** Return the connection to the pool manually, this is optional */
	void ConnectionPool::returnConnection(
		seastar::lw_shared_ptr<Connection>&& connection, ConnectionStream&& stream) {
		stream.close();
		if (!waiters_.empty()) {
			auto promise = waiters_.pop();
			auto stream = connection->openStream();
			if (CQL_UNLIKELY(!stream.isValid())) {
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
		metricsData_(seastar::make_lw_shared<MetricsData>()),
		allConnections_(),
		connectingCount_(0),
		waiters_(static_cast<std::size_t>(-1)),
		findIdleConnectionTimerIsRunning_(false),
		dropIdleConnectionTimerIsRunning_(false) { }

	/** Spawn a new connection */
	void ConnectionPool::spawnConnection() {
		++connectingCount_;
		auto self = shared_from_this();
		(void)seastar::do_with(
			std::move(self),
			static_cast<std::size_t>(0),
			[] (auto& self, auto& count) {
			return seastar::repeat([&self, &count] {
				// choose one node and create connection
				auto node = self->nodeCollection_->chooseOneNode();
				auto connection = seastar::make_lw_shared<Connection>(
					self->sessionConfiguration_, node, self->metricsData_);
				// initialize connection
				return connection->ready().then([&self, node, connection] {
					// initialize connection success
					self->nodeCollection_->reportSuccess(node);
					self->allConnections_.emplace_back(connection);
					self->feedWaiters();
					self->metricsData_->pool_connections_total += 1;
					self->metricsData_->pool_connections_current = self->allConnections_.size();
					return seastar::stop_iteration::yes;
				}).handle_exception([&self, &count, node] (std::exception_ptr ex) {
					// initialize connection failed, try next node until all tried
					self->nodeCollection_->reportFailure(node);
					if (++count >= self->nodeCollection_->getNodesCount()) {
						self->cleanWaiters(ex);
						return seastar::stop_iteration::yes;
					} else {
						return seastar::stop_iteration::no;
					}
				});
			}).finally([&self] {
				--self->connectingCount_;
			});
		});
	}

	/** Add a new waiter */
	seastar::future<seastar::lw_shared_ptr<Connection>, ConnectionStream>
		ConnectionPool::addWaiter() {
		seastar::promise<seastar::lw_shared_ptr<Connection>, ConnectionStream> promise;
		auto future = promise.get_future();
		if (CQL_UNLIKELY(!waiters_.push(std::move(promise)))) {
			return seastar::make_exception_future<
				seastar::lw_shared_ptr<Connection>, ConnectionStream>(
				LogicException(CQL_CODEINFO, "push connection waiter to queue failed"));
		}
		return future;
	}

	/** Find idle connection and feed waiters */
	void ConnectionPool::feedWaiters() {
		while (!waiters_.empty()) {
			auto result = tryGetConnection();
			if (result.first.get() != nullptr) {
				waiters_.pop().set_value(
					std::move(result.first), std::move(result.second));
			} else {
				break;
			}
		}
	}

	/** Tell all waiters that an error has occurred */
	void ConnectionPool::cleanWaiters(const std::exception_ptr& ex) {
		while (!waiters_.empty()) {
			waiters_.pop().set_exception(ex);
		}
	}

	/** Timer used to find idle connection and feed waiters */
	void ConnectionPool::findIdleConnectionTimer() {
		static const std::size_t SleepInterval = 1;
		if (findIdleConnectionTimerIsRunning_) {
			return;
		}
		findIdleConnectionTimerIsRunning_ = true;
		auto self = shared_from_this();
		(void)seastar::do_with(std::move(self), [] (auto& self) {
			return seastar::repeat([&self] {
				return seastar::sleep(std::chrono::milliseconds(SleepInterval)).then([&self] {
					// feed waiters
					self->feedWaiters();
					if (self->waiters_.empty()) {
						// stop timer after no waiters
						return seastar::stop_iteration::yes;
					} else if (self->allConnections_.size() + self->connectingCount_ <
						self->sessionConfiguration_->getMinPoolSize()) {
						// spawn connection to prevent waiting indefinitely
						self->spawnConnection();
					}
					return seastar::stop_iteration::no;
				});
			}).finally([&self] {
				self->findIdleConnectionTimerIsRunning_ = false;
				if (!self->waiters_.empty()) {
					// prevent task race
					self->findIdleConnectionTimer();
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
		(void)seastar::do_with(std::move(self), [] (auto& self) {
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
					self->metricsData_->pool_connections_current = self->allConnections_.size();
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

