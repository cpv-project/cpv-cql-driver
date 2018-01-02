#include <CqlDriver/Exceptions/CqlNotImplementedException.hpp>
#include "CqlTcpConnector.hpp"

namespace cql {
	/* Setup the connection */
	seastar::future<seastar::connected_socket> CqlTcpConnector::connect(
		const seastar::socket_address& address) const {
		// TODO
		throw CqlNotImplementedException(CQL_CODEINFO);
	}

	/** Constructor */
	CqlTcpConnector::CqlTcpConnector() { }
}

