#pragma once
#include <seastar/core/future.hh>
#include <seastar/net/api.hh>
#include <CQLDriver/Common/NodeConfiguration.hpp>

namespace cql {
	/** The interface used to setup a connection to cql database server */
	class ConnectorBase {
	public:
		/* Setup the connection */
		virtual seastar::future<seastar::connected_socket> connect(
			const NodeConfiguration& nodeConfiguration,
			const seastar::socket_address& address) const = 0;

		/** Virtual destructor */
		virtual ~ConnectorBase() = default;
	};
}

