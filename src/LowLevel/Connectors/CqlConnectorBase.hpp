#pragma once
#include <core/future.hh>
#include <net/api.hh>
#include <CqlDriver/Common/CqlNodeConfiguration.hpp>

namespace cql {
	/** The interface used to setup a connection to cql database server */
	class CqlConnectorBase {
	public:
		/* Setup the connection */
		virtual seastar::future<seastar::connected_socket> connect(
			const CqlNodeConfiguration& nodeConfiguration,
			const seastar::socket_address& address) const = 0;

		/** Virtual destructor */
		virtual ~CqlConnectorBase() = default;
	};
}

