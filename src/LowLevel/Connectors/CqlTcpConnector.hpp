#pragma once
#include <core/future.hh>
#include <net/api.hh>
#include "CqlConnectorBase.hpp"

namespace cql {
	/** Connect database server with plain tcp connection */
	class CqlTcpConnector : public CqlConnectorBase {
	public:
		/* Setup the connection */
		seastar::future<seastar::connected_socket> connect(
			const seastar::socket_address& address) const override;

		/** Constructor */
		CqlTcpConnector();
	};
}

