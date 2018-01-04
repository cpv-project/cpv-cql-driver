#include <CqlDriver/Exceptions/CqlNotImplementedException.hpp>
#include <core/reactor.hh>
#include "CqlTcpConnector.hpp"

namespace cql {
	/* Setup the connection */
	seastar::future<seastar::connected_socket> CqlTcpConnector::connect(
		const seastar::socket_address& address) const {
		return seastar::engine().net().connect(address);
	}

	/** Constructor */
	CqlTcpConnector::CqlTcpConnector() { }
}

