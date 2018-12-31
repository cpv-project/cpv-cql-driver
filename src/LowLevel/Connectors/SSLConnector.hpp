#pragma once
#include <string>
#include <seastar/core/sharded.hh>
#include <seastar/core/future.hh>
#include <seastar/core/shared_future.hh>
#include <seastar/net/api.hh>
#include <seastar/net/tls.hh>
#include "./ConnectorBase.hpp"

namespace cql {
	/** Connection database server with ssl encryped tcp connection */
	class SSLConnector : public ConnectorBase {
	public:
		/* Setup the connection */
		seastar::future<seastar::connected_socket> connect(
			const NodeConfiguration& nodeConfiguration,
			const seastar::socket_address& address) const override;

		/** Constructor */
		SSLConnector();

	private:
		seastar::shared_ptr<seastar::tls::certificate_credentials> certificates_;
		seastar::shared_future<> initialized_;
	};
}

