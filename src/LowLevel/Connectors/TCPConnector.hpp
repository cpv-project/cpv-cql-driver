#pragma once
#include <seastar/core/future.hh>
#include <seastar/net/api.hh>
#include "./ConnectorBase.hpp"

namespace cql {
	/** Connect database server with plain tcp connection */
	class TCPConnector : public ConnectorBase {
	public:
		/* Setup the connection */
		seastar::future<seastar::connected_socket> connect(
			const NodeConfiguration& nodeConfiguration,
			const seastar::socket_address& address) const override;

		/** Constructor */
		TCPConnector();
	};
}

