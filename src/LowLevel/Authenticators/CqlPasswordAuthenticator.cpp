#include <CqlDriver/Common/Exceptions/CqlNotImplementedException.hpp>
#include "CqlPasswordAuthenticator.hpp"

namespace cql {
	/** Perform the authentication */
	seastar::future<> CqlPasswordAuthenticator::authenticate(CqlConnection&) const {
		// TODO
		throw CqlNotImplementedException(CQL_CODEINFO);
	}
}

