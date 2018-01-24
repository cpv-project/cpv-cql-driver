#include <limits>
#include <net/dns.hh>
#include <core/reactor.hh>
#include <CqlDriver/Common/Exceptions/CqlNotImplementedException.hpp>
#include <CqlDriver/Common/Exceptions/CqlNetworkException.hpp>
#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include <CqlDriver/Common/Exceptions/CqlConnectionInitializeException.hpp>
#include "./Connectors/CqlConnectorFactory.hpp"
#include "./Authenticators/CqlAuthenticatorFactory.hpp"
#include "./RequestMessages/CqlRequestMessageFactory.hpp"
#include "./RequestMessages/CqlOptionsMessage.hpp"
#include "./RequestMessages/CqlStartupMessage.hpp"
#include "./ResponseMessages/CqlResponseMessageFactory.hpp"
#include "CqlConnection.hpp"

#include <core/sleep.hh>

namespace cql {
	/** Initialize connection and wait until it's ready to send ordinary messages */
	seastar::future<> CqlConnection::ready() {
		if (isReady_) {
			return seastar::make_ready_future<>();
		}
		auto self = shared_from_this();
		return seastar::futurize_apply([self] {
			// connect to database server
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
			// connect successful
			self->socket_ = SocketHolder(std::move(fd));
		}).handle_exception([self] (std::exception_ptr ex) {
			// connect failed
			return seastar::make_exception_future(CqlNetworkException(
				CQL_CODEINFO, "connect to",
				self->nodeConfiguration_->getAddress().first, "failed:\n", ex));
		}).then([self] {
			// send OPTION
			auto optionMessage = CqlRequestMessageFactory::makeRequestMessage<CqlOptionsMessage>();
			return self->sendMessage(std::move(optionMessage), self->streamZero_);
		}).then([self] {
			// receive SUPPORTED
			return self->waitNextMessage(self->streamZero_);
		}).then([self] (auto message) {
			// send STARTUP
			if (message->getHeader().getOpCode() != CqlMessageType::Supported) {
				return seastar::make_exception_future(CqlLogicException(
					CQL_CODEINFO, "unexcepted response to OPTION message:", message->toString()));
			}
			auto startupMessage = CqlRequestMessageFactory::makeRequestMessage<CqlStartupMessage>();
			return self->sendMessage(std::move(startupMessage), self->streamZero_);
		}).then([self] {
			// perform authentication
			return self->authenticator_->authenticate(self, self->streamZero_);
		}).then([self] {
			// the connection is ready
			self->isReady_ = true;
		}).handle_exception([self] (std::exception_ptr ex) {
			// initialize failed
			return seastar::make_exception_future(CqlConnectionInitializeException(
				CQL_CODEINFO, "initialize connection to",
				self->nodeConfiguration_->getAddress().first, "failed:\n", ex));
		});
	}

	/** Open a new stream */
	CqlConnectionStream CqlConnection::openStream() {
		if (freeStreamIds_->empty()) {
			// no available stream
			return CqlConnectionStream();
		} else {
			auto streamId = freeStreamIds_->back();
			freeStreamIds_->pop_back();
			return CqlConnectionStream(streamId, freeStreamIds_);
		}
	}

	/** Get how many free streams available */
	std::size_t CqlConnection::getFreeStreamsCount() const {
		return freeStreamIds_->size();
	}

	/** Send a message to the given stream and wait for success */
	seastar::future<> CqlConnection::sendMessage(
		CqlObject<CqlRequestMessageBase>&& message,
		const CqlConnectionStream& stream) {
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
				return self->socket_.out().write(self->sendingBuffer_).then([&self] {
					return self->socket_.out().flush();
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
		const CqlConnectionStream& stream) {
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
					return self->socket_.in().read_exactly(self->connectionInfo_.getHeaderSize())
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
						return self->socket_.in().read_exactly(bodyLength)
						.then([&self, header=std::move(header)] (seastar::temporary_buffer<char> buf) mutable {
							const char* ptr = buf.begin();
							const char* end = buf.end();
							auto message = CqlResponseMessageFactory::makeResponseMessage(std::move(header));
							message->decodeBody(self->connectionInfo_, ptr, end);
							// find the corresponding promise
							auto streamId = message->getHeader().getStreamId();
							if (streamId >= self->receivedMessageQueueMap_.size()) {
								self->close(joinString(" ",
									"stream id of received message out of range:", streamId));
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
									self->close(joinString(" ",
										"max pending messages is reached, stream id:", streamId));
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
		const seastar::lw_shared_ptr<CqlSessionConfiguration>& sessionConfiguration,
		const seastar::lw_shared_ptr<CqlNodeConfiguration>& nodeConfiguration) :
		CqlConnection(
			sessionConfiguration,
			nodeConfiguration,
			CqlConnectorFactory::getConnector(*nodeConfiguration),
			CqlAuthenticatorFactory::getAuthenticator(*nodeConfiguration)) { }

	/** Constructor */
	CqlConnection::CqlConnection(
		const seastar::lw_shared_ptr<CqlSessionConfiguration>& sessionConfiguration,
		const seastar::lw_shared_ptr<CqlNodeConfiguration>& nodeConfiguration,
		const seastar::shared_ptr<CqlConnectorBase>& connector,
		const seastar::shared_ptr<CqlAuthenticatorBase>& authenticator) :
		sessionConfiguration_(sessionConfiguration),
		nodeConfiguration_(nodeConfiguration),
		connector_(connector),
		authenticator_(authenticator),
		socket_(),
		isReady_(false),
		connectionInfo_(),
		freeStreamIds_(seastar::make_lw_shared<decltype(freeStreamIds_)::element_type>()),
		streamZero_(0, freeStreamIds_),
		sendingFuture_(seastar::make_ready_future<>()),
		sendingBuffer_(),
		receivingPromiseMap_(nodeConfiguration_->getMaxStreams()),
		receivedMessageQueueMap_(),
		receivingPromiseCount_(0) {
		// initialize free stream ids, exclude stream zero (which is for internal communication)
		freeStreamIds_->resize(nodeConfiguration_->getMaxStreams() - 1);
		for (std::size_t i = 0, j = freeStreamIds_->size(); i < j; ++i) {
			(*freeStreamIds_)[i] = j-i;
		}
		// initialize received message queues
		for (std::size_t i = 0, j = nodeConfiguration_->getMaxStreams(); i < j; ++i) {
			receivedMessageQueueMap_.emplace_back(nodeConfiguration->getMaxPendingMessages());
		}
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
		// close the socket and reset the ready state
		socket_ = SocketHolder();
		isReady_ = false;
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

