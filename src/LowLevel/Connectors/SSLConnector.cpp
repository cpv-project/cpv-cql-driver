#include <CQLDriver/Common/CommonDefinitions.hpp>
#include "./SSLConnector.hpp"

namespace cql {
	/* Setup the connection */
	seastar::future<seastar::connected_socket> SSLConnector::connect(
		const NodeConfiguration&,
		const seastar::socket_address& address) const {
		if (CQL_LIKELY(initialized_.available() && !initialized_.failed())) {
			// fast path
			return seastar::tls::connect(certificates_, address, "");
		} else {
			// slow path
			return initialized_.get_future().then([c = certificates_, a = address] {
				return seastar::tls::connect(c, a, "");
			});
		}
	}

	/** Constructor */
	SSLConnector::SSLConnector() :
		certificates_(seastar::make_shared<seastar::tls::certificate_credentials>()),
		initialized_(certificates_->set_system_trust()) { }
}

