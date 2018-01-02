#include "CqlConnection.hpp"

namespace cql {
	/** Wait for connection ready */
	seastar::future<> CqlConnection::ready() {
		if (isReady_) {
			return seastar::make_ready_future<>();
		}
		return seastar::engine().net().connect(address_).then([this] (seastar::connected_socket&& fd) {
			socket_ = std::move(fd);
			isReady_ = true;
		});
	}

	/** Constructor */
	CqlConnection::CqlConnection(
		const seastar::socket_address& address,
		bool connectWithSsl,
		const seastar::shared_ptr<CqlAuthenticatorBase>& authenticator) :
		address_(address),
		connectWithSsl_(connectWithSsl),
		authenticator_(authenticator),
		socket_(),
		isReady_(false) { }
}

