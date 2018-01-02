#pragma once
#include <memory>
#include <net/api.hh>
#include <core/reactor.hh>
#include "Authenticators/CqlAuthenticatorBase.hpp"

namespace cql {
	/**
	 * Map to single tcp connection to the database
	 * Example:
	 * TODO
	 */
	class CqlConnection {
	public:
		/** Wait for connection ready */
		seastar::future<> ready();

		/** Constructor */
		CqlConnection(
			const seastar::socket_address& address,
			bool connectWithSsl,
			const seastar::shared_ptr<CqlAuthenticatorBase>& authenticator);

	private:
		seastar::socket_address address_;
		bool connectWithSsl_;
		seastar::shared_ptr<CqlAuthenticatorBase> authenticator_;

		seastar::connected_socket socket_;
		bool isReady_;
	};
}

