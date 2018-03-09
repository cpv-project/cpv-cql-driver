#pragma once
#include <string>
#include <core/sharded.hh>
#include <core/future.hh>
#include <core/shared_future.hh>
#include <net/api.hh>
#include <net/tls.hh>
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

