#include <limits>
#include <net/dns.hh>
#include <core/reactor.hh>
#include <CqlDriver/Common/Exceptions/CqlNotImplementedException.hpp>
#include <CqlDriver/Common/Exceptions/CqlNetworkException.hpp>
#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include "./Connectors/CqlConnectorFactory.hpp"
#include "./Authenticators/CqlAuthenticatorFactory.hpp"
#include "./RequestMessages/CqlRequestMessageFactory.hpp"
#include "./ResponseMessages/CqlResponseMessageFactory.hpp"
#include "CqlConnection.hpp"

namespace cql {
	/** Constructor */
	CqlConnection::Stream::Stream(std::uint16_t streamId, const seastar::lw_shared_ptr<State>& state) :
		streamId_(streamId),
		state_(state) {
		if (state_.get() != nullptr) {
			if (state_->isInUse) {
				throw CqlLogicException(CQL_CODEINFO, "construct stream with a already in use state");
			}
			state_->isInUse = true;
		}
	}

	/** Move constructor */
	CqlConnection::Stream::Stream(CqlConnection::Stream&& stream) :
		streamId_(stream.streamId_),
		state_(std::move(stream.state_)) {
		stream.state_ = nullptr;
	}

	/** Move assignment */
	CqlConnection::Stream& CqlConnection::Stream::operator=(CqlConnection::Stream&& stream) {
		if (&stream != this) {
			new (this) CqlConnection::Stream(std::move(stream));
		}
		return *this;
	}

	/** Destructor */
	CqlConnection::Stream::~Stream() {
		if (state_.get() != nullptr) {
			state_->isInUse = false;
		}
	}
	
	/** Initialize connection and wait until it's ready to send ordinary messages */
	seastar::future<> CqlConnection::ready() {
		if (isReady_) {
			return seastar::make_ready_future<>();
		}
		auto self = shared_from_this();
		return seastar::futurize_apply([self] {
			// build connection
			seastar::socket_address address;
			if (self->nodeConfiguration_->getIpAddress(
				address, self->sessionConfiguration_->getDnsCacheTime())) {
				return self->connector_->connect(*self->nodeConfiguration_, address);
			} else {
				return seastar::net::dns::resolve_name(self->nodeConfiguration_->getAddress().first)
					.then([self](seastar::net::inet_address inetAddress) {
					if (inetAddress.in_family() == seastar::net::inet_address::family::INET) {
						seastar::socket_address address(seastar::ipv4_addr(
							inetAddress, self->nodeConfiguration_->getAddress().second));
						self->nodeConfiguration_->updateIpAddress(address);
						return self->connector_->connect(*self->nodeConfiguration_, address);
					} else {
						// seastar's socket_address only support ipv4 for now
						return seastar::make_exception_future<seastar::connected_socket>(
							CqlNotImplementedException(CQL_CODEINFO, "ipv6 address is unsupported yet"));
					}
				});
			}
		}).then([self] (seastar::connected_socket fd) {
			self->socket_ = std::move(fd);
			self->readStream_ = self->socket_.input();
			self->writeStream_ = self->socket_.output();
			self->isConnected_ = true;
		}).handle_exception([self] (std::exception_ptr ex) {
			return seastar::make_exception_future(CqlNetworkException(
				CQL_CODEINFO, "connect to",
				self->nodeConfiguration_->getAddress().first, "failed:", ex));
		}).then([self] {
			// send OPTION

		}).then([self] {
			// receive SUPPORTED

		}).then([self] {
			// send ready

		}).then([self] {
			// perform authentication

		}).then([self] {
			// ready now
			self->isReady_ = true;
		});
	}

	/** Open a new stream */
	CqlConnection::Stream CqlConnection::openStream() {
		// OPTIMIZE: it's a O(n) search, can we do better here?
		for (std::size_t i = 1, j = streamStates_.size(); i < j; ++i) {
			std::size_t nextStreamId = (lastOpenedStream_ + i) % j;
			auto& state = streamStates_[nextStreamId];
			if (state.get() == nullptr) {
				// lazy initialize
				state = seastar::make_lw_shared<CqlConnection::Stream::State>();
			}
			if (!state->isInUse) {
				lastOpenedStream_ = nextStreamId;
				return CqlConnection::Stream(nextStreamId, state);
			}
		}
		return CqlConnection::Stream(0, nullptr);
	}

	/** Send a message to the given stream and wait for success */
	seastar::future<> CqlConnection::sendMessage(
		CqlObject<CqlRequestMessageBase>&& message, const CqlConnection::Stream& stream) {
		// check connection state
		if (!isConnected_) {
			return seastar::make_exception_future(
				CqlNetworkException(CQL_CODEINFO,
				"connection is not connected, either ready() is not called or it's closed"));
		}
		// ensure only one message is sending at the same time
		message->getHeader().setStreamId(stream.getStreamId());
		auto previousSendingFuture = std::move(sendingFuture_);
		seastar::promise<> sendingPromise;
		sendingFuture_ = sendingPromise.get_future();
		auto self = shared_from_this();
		return seastar::do_with(
			std::move(self),
			std::move(message),
			std::move(sendingPromise),
			std::move(previousSendingFuture), [] (
			auto& self,
			auto& message,
			auto& sendingPromise,
			auto& previousSendingFuture) {
			return previousSendingFuture.then([&self, &message] {
				// encode message to binary data
				self->sendingBuffer_.resize(0);
				message->getHeader().encodeHeaderPre(self->connectionInfo_, self->sendingBuffer_);
				message->encodeBody(self->connectionInfo_, self->sendingBuffer_);
				message->getHeader().encodeHeaderPost(self->connectionInfo_, self->sendingBuffer_);
				// send the encoded binary data
				return self->writeStream_.write(self->sendingBuffer_).then([&self] {
					return self->writeStream_.flush();
				});
			}).then([&self, &sendingPromise] {
				// this message is successful, allow next message to start sending
				sendingPromise.set_value();
			}).handle_exception([&self, &sendingPromise] (std::exception_ptr ex) {
				// this message is failed, report the error to both waiters
				sendingPromise.set_exception(ex);
				return seastar::make_exception_future(CqlNetworkException(
					CQL_CODEINFO, "send message to",
					self->nodeConfiguration_->getAddress().first, "failed:", ex));
			});
		});
	}

	/** Wait for the next message from the given stream */
	seastar::future<CqlObject<CqlResponseMessageBase>> CqlConnection::waitNextMessage(
		const CqlConnection::Stream& stream) {
		// check connection state
		if (!isConnected_) {
			return seastar::make_exception_future<CqlObject<CqlResponseMessageBase>>(
				CqlNetworkException(CQL_CODEINFO,
				"connection is not connected, either ready() is not called or it's closed"));
		}
		// try getting message directly from received queue
		auto streamId = stream.getStreamId();
		auto& queue = receivedMessageQueueMap_.at(streamId);
		if (!queue.empty()) {
			return seastar::make_ready_future<CqlObject<CqlResponseMessageBase>>(queue.pop());
		}
		// message not available, try receiving it from network
		auto& promiseSlot = receivingPromiseMap_.at(streamId);
		if (promiseSlot.first) {
			return seastar::make_exception_future<CqlObject<CqlResponseMessageBase>>(
				CqlLogicException(CQL_CODEINFO, "there already a message waiter registered for this stream"));
		}
		promiseSlot.first = true;
		promiseSlot.second = {};
		if (receivingPromiseCount_++ == 0) {
			// the first who made promise has responsibility to start the receiver
			auto self = shared_from_this();
			seastar::do_with(std::move(self), [](auto& self) {
				return seastar::repeat([&self] {
					// receive message header
					return self->readStream_.read_exactly(self->connectionInfo_.getHeaderSize())
					.then([&self] (seastar::temporary_buffer<char> buf) {
						const char* ptr = buf.begin();
						const char* end = buf.end();
						CqlMessageHeader header;
						header.decodeHeader(self->connectionInfo_, ptr, end);
						auto bodyLength = header.getBodyLength();
						if (bodyLength > self->connectionInfo_.getMaximumMessageBodySize()) {
							self->close(joinString(" ", "message body too large:", bodyLength));
							return seastar::make_ready_future<seastar::stop_iteration>(
								seastar::stop_iteration::yes);
						}
						// receive message body
						return self->readStream_.read_exactly(bodyLength)
						.then([&self, header=std::move(header)] (seastar::temporary_buffer<char> buf) mutable {
							const char* ptr = buf.begin();
							const char* end = buf.end();
							auto message = CqlResponseMessageFactory::makeResponseMessage(std::move(header));
							message->decodeBody(self->connectionInfo_, ptr, end);
							// find the corresponding promise
							auto streamId = message->getHeader().getStreamId();
							if (streamId >= self->receivedMessageQueueMap_.size()) {
								self->close(joinString(" ", "stream id of received message out of range:", streamId));
								return seastar::stop_iteration::yes;
							}
							auto& promiseSlot = self->receivingPromiseMap_[streamId];
							if (promiseSlot.first) {
								// pass message directly to the promise
								promiseSlot.first = false;
								promiseSlot.second.set_value(std::move(message));
								if (self->receivingPromiseCount_ == 0) {
									self->close("incorrect receiving promise count, should be logic error");
									return seastar::stop_iteration::yes;
								}
								--self->receivingPromiseCount_;
							} else {
								// enqueue message to received queue
								if (!self->receivedMessageQueueMap_[streamId].push(std::move(message))) {
									self->close(joinString(" ", "max pending messages is reached, stream id:", streamId));
									return seastar::stop_iteration::yes;
								}
							}
							if (self->receivingPromiseCount_ == 0) {
								// no more message waiter is waiting, exit receiver
								return seastar::stop_iteration::yes;
							} else {
								// continue to receive next message
								return seastar::stop_iteration::no;
							}
						});
					});
				}).handle_exception([&self] (std::exception_ptr ex) {
					self->close(joinString(" ", "receive message failed:", ex));
				});
			});
		}
		return promiseSlot.second.get_future();
	}

	/** Constructor */
	CqlConnection::CqlConnection(
		const seastar::shared_ptr<CqlSessionConfiguration>& sessionConfiguration,
		const seastar::shared_ptr<CqlNodeConfiguration>& nodeConfiguration) :
		CqlConnection(
			sessionConfiguration,
			nodeConfiguration,
			CqlConnectorFactory::getConnector(*nodeConfiguration),
			CqlAuthenticatorFactory::getAuthenticator(*nodeConfiguration)) { }

	/** Constructor */
	CqlConnection::CqlConnection(
		const seastar::shared_ptr<CqlSessionConfiguration>& sessionConfiguration,
		const seastar::shared_ptr<CqlNodeConfiguration>& nodeConfiguration,
		const seastar::shared_ptr<CqlConnectorBase>& connector,
		const seastar::shared_ptr<CqlAuthenticatorBase>& authenticator) :
		sessionConfiguration_(sessionConfiguration),
		nodeConfiguration_(nodeConfiguration),
		connector_(connector),
		authenticator_(authenticator),
		socket_(),
		readStream_(),
		writeStream_(),
		isConnected_(false),
		isReady_(false),
		connectionInfo_(),
		streamStates_(nodeConfiguration_->getMaxStreams()),
		streamZero_(0, nullptr),
		lastOpenedStream_(0),
		sendingFuture_(seastar::make_ready_future<>()),
		sendingBuffer_(),
		receivingPromiseMap_(nodeConfiguration_->getMaxStreams()),
		receivedMessageQueueMap_(),
		receivingPromiseCount_(0) {
		// initialize received message queues
		for (std::size_t i = 0, j = nodeConfiguration_->getMaxStreams(); i < j; ++i) {
			receivedMessageQueueMap_.emplace_back(nodeConfiguration->getMaxPendingMessages());
		}
		// open stream zero, which is for internal communication
		auto state = seastar::make_lw_shared<Stream::State>();
		streamStates_.at(0) = state;
		streamZero_ = Stream(0, state);
	}

	/** Destructor */
	CqlConnection::~CqlConnection() {
		try {
			close("close from destructor");
		} catch (...) {
			std::cerr << std::current_exception() << std::endl;
		}
	}

	/** Close the connection */
	void CqlConnection::close(const seastar::sstring& errorMessage) {
		if (!isConnected_) {
			return;
		}
		isConnected_ = false;
		isReady_ = false;
		readStream_ = {};
		writeStream_ = {};
		socket_ = {};
		// notify all message waiters
		receivingPromiseCount_ = 0;
		for (auto& slot : receivingPromiseMap_) {
			if (slot.first) {
				slot.first = false;
				slot.second.set_exception(CqlNetworkException(CQL_CODEINFO, errorMessage));
			}
		}
	}
}

