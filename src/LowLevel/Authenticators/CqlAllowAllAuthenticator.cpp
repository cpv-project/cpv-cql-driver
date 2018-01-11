#include <CqlDriver/Common/Exceptions/CqlNotImplementedException.hpp>
#include "CqlAllowAllAuthenticator.hpp"

namespace cql {
	/** Perform the authentication */
	seastar::future<> CqlAllowAllAuthenticator::authenticate(CqlConnection&) const {
		// TODO
		throw CqlNotImplementedException(CQL_CODEINFO);
	}
}

