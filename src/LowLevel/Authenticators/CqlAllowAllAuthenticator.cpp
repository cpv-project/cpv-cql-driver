#include <CqlDriver/Common/Exceptions/CqlNotImplementedException.hpp>
#include "../CqlConnection.hpp"
#include "CqlAllowAllAuthenticator.hpp"

namespace cql {
	/** Perform the authentication */
	seastar::future<> CqlAllowAllAuthenticator::authenticate(
		const seastar::shared_ptr<CqlConnection>& connection,
		const CqlConnectionStream& stream) const {
		// TODO
		throw CqlNotImplementedException(CQL_CODEINFO);
	}
}

