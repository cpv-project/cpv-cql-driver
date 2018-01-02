#include <CqlDriver/Exceptions/CqlNotImplementedException.hpp>
#include "CqlEmptyAuthenticator.hpp"

namespace cql {
	/** Perform the authentication */
	seastar::future<> CqlEmptyAuthenticator::authenticate(CqlConnection&) const {
		// TODO
		throw CqlNotImplementedException(CQL_CODEINFO);
	}
}

