#include <CqlDriver/Exceptions/CqlNotImplementedException.hpp>
#include "CqlSslConnector.hpp"

namespace cql {
	/* Setup the connection */
	seastar::future<seastar::connected_socket> CqlSslConnector::connect(
		const seastar::socket_address& address) const {
		// TODO
		throw CqlNotImplementedException(CQL_CODEINFO);
	}

	/** Constructor */
	CqlSslConnector::CqlSslConnector(const std::string& pem) :
		pem_(pem),
		certificates_(seastar::make_shared<seastar::tls::certificate_credentials>()) {
		certificates_->set_x509_trust(pem_, seastar::tls::x509_crt_format::PEM);
	}
}

