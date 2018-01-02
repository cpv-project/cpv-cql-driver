#include <CqlDriver/LowLevel/CqlConnection.hpp>

namespace cql {
	/** Wait for connection ready */
	seastar::future<> CqlConnection::ready() {
		return seastar::make_ready_future<>();
	}

	/** Constructor */
	CqlConnection::CqlConnection(
		const seastar::socket_address& address,
		bool connectWithSsl,
		const seastar::shared_ptr<CqlAuthenticatorBase>& authenticator) :
		address_(address),
		connectWithSsl_(connectWithSsl),
		authenticator_(authenticator) { }
}

