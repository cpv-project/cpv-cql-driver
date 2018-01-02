#pragma once
#include <string>
#include <core/sharded.hh>
#include <core/future.hh>
#include <net/api.hh>
#include <net/tls.hh>
#include "CqlConnectorBase.hpp"

namespace cql {
	/** TODO */
	class CqlSslConnector : public CqlConnectorBase {
	public:
		/* Setup the connection */
		seastar::future<seastar::connected_socket> connect(
			const seastar::socket_address& address) const override;

		/** Constructor */
		CqlSslConnector(const std::string& pem);

	private:
		std::string pem_;
		seastar::shared_ptr<seastar::tls::certificate_credentials> certificates_;
	};
}

