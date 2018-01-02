#include <CqlDriver/Exceptions/CqlNetworkException.hpp>
#include "CqlConnection.hpp"

namespace cql {
	/** Wait for connection ready */
	seastar::future<> CqlConnection::ready() {
		if (isReady_) {
			return seastar::make_ready_future<>();
		}
		return seastar::futurize_apply([this] {
			return connector_->connect(address_);
		}).then([this] (seastar::connected_socket&& fd) {
			socket_ = std::move(fd);
			isReady_ = true;
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

