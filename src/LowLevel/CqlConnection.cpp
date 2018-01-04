#include <CqlDriver/Exceptions/CqlNetworkException.hpp>
#include "CqlConnection.hpp"

namespace cql {
	/** Wait for connection ready */
	seastar::future<> CqlConnection::ready() {
		if (isReady_) {
			return seastar::make_ready_future<>();
		}
		auto self = shared_from_this();
		return seastar::futurize_apply([self] {
			return self->connector_->connect(self->address_);
		}).then([self] (seastar::connected_socket&& fd) {
			self->socket_ = std::move(fd);
			self->isReady_ = true;
		}).then([] {
			std::cout << "connect success" << std::endl;
		}).handle_exception([] (std::exception_ptr ex) {
			throw CqlNetworkException(CQL_CODEINFO, ex);
		});
	}

	/** Constructor */
	CqlConnection::CqlConnection(
		const seastar::socket_address& address,
		const seastar::shared_ptr<CqlConnectorBase>& connector,
		const seastar::shared_ptr<CqlAuthenticatorBase>& authenticator) :
		address_(address),
		connector_(connector),
		authenticator_(authenticator),
		socket_(),
		isReady_(false) { }
}

