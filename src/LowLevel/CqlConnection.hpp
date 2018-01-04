#pragma once
#include <memory>
#include <net/api.hh>
#include <core/reactor.hh>
#include "Connectors/CqlConnectorBase.hpp"
#include "Authenticators/CqlAuthenticatorBase.hpp"

namespace cql {
	/**
	 * Map to single tcp connection to the database
	 * Example:
	 * TODO
	 */
	class CqlConnection :
		public seastar::enable_shared_from_this<CqlConnection> {
	public:
		/** Wait for connection ready */
		seastar::future<> ready();

		/** Constructor */
		CqlConnection(
			const seastar::socket_address& address,
			const seastar::shared_ptr<CqlConnectorBase>& connector,
			const seastar::shared_ptr<CqlAuthenticatorBase>& authenticator);

	private:
		seastar::socket_address address_;
		seastar::shared_ptr<CqlConnectorBase> connector_;
		seastar::shared_ptr<CqlAuthenticatorBase> authenticator_;

		seastar::connected_socket socket_;
		bool isReady_;
	};
}

