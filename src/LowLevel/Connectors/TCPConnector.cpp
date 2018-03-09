#include <core/reactor.hh>
#include "./TCPConnector.hpp"

namespace cql {
	/* Setup the connection */
	seastar::future<seastar::connected_socket> TCPConnector::connect(
		const NodeConfiguration&,
		const seastar::socket_address& address) const {
		return seastar::engine().net().connect(address);
	}

	/** Constructor */
	TCPConnector::TCPConnector() { }
}

