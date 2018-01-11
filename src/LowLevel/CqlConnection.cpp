#include <net/dns.hh>
#include <CqlDriver/Common/Exceptions/CqlNotImplementedException.hpp>
#include <CqlDriver/Common/Exceptions/CqlNetworkException.hpp>
#include "./Connectors/CqlConnectorFactory.hpp"
#include "./Authenticators/CqlAuthenticatorFactory.hpp"
#include "CqlConnection.hpp"

namespace cql {
	/** Wait for connection ready */
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
		});

		
		/* auto self = shared_from_this();
		return seastar::futurize_apply([self] {
			return self->connector_->connect(self->address_);
		}).then([self] (seastar::connected_socket&& fd) {
			self->socket_ = std::move(fd);
			self->isReady_ = true;
		}).handle_exception([self] (std::exception_ptr ex) {
			throw CqlNetworkException(CQL_CODEINFO, "connect to", self->address_, "failed:", ex);
		}); */
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
		isReady_(false) { }
}

