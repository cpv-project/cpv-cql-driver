#pragma once
#include <memory>
#include <net/api.hh>
#include "CqlAuthenticatorBase.hpp"

namespace cql {
	/**
	 * Map to single tcp connection to the database
	 * Example:
	 * TODO
	 */
	class CqlConnection {
	public:
		/** Constructor */
		CqlConnection(
			const seastar::socket_address& address,
			bool connectWithSsl,
			const seastar::shared_ptr<CqlAuthenticatorBase>& authenticator);

		/** Wait for connection ready */
		seastar::future<> ready();

	private:
		seastar::socket_address address_;
		bool connectWithSsl_;
		seastar::shared_ptr<CqlAuthenticatorBase> authenticator_;
	};
}

