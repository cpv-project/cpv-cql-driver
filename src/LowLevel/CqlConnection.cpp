#include <net/dns.hh>
#include <core/reactor.hh>
#include <CqlDriver/Common/Exceptions/CqlNotImplementedException.hpp>
#include <CqlDriver/Common/Exceptions/CqlNetworkException.hpp>
#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include "./Connectors/CqlConnectorFactory.hpp"
#include "./Authenticators/CqlAuthenticatorFactory.hpp"
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
						// seastar's socket_address only support ipv4 now so throw a exception for ipv6
						throw CqlNotImplementedException(CQL_CODEINFO, "ipv6 address isn't supported yet");
					}
				});
			}
		}).then([self] (seastar::connected_socket fd) {
			self->socket_ = std::move(fd);
		}).handle_exception([self] (std::exception_ptr ex) {
			throw CqlNetworkException(CQL_CODEINFO, "connect to",
				self->nodeConfiguration_->getAddress().first, "failed:", ex);
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
		isReady_(false),
		connectionInfo_(),
		streamStates_(nodeConfiguration_->getMaxStream()),
		streamZero_(0, nullptr),
		lastOpenedStream_(0),
		sendPromiseQueue_(nodeConfiguration_->getMaxStream()),
		sendPromiseMap_(nodeConfiguration_->getMaxStream()),
		receivePromiseCount_(0),
		receivePromiseMap_(nodeConfiguration_->getMaxStream()) {
		// open stream zero, which is for internal communication
		auto state = seastar::make_lw_shared<Stream::State>();
		streamStates_.at(0) = state;
		streamZero_ = Stream(0, state);
	}
}

