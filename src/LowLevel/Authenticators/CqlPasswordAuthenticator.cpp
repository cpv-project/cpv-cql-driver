#include <CqlDriver/Common/Exceptions/CqlNotImplementedException.hpp>
#include "CqlPasswordAuthenticator.hpp"

namespace cql {
	/** Perform the authentication */
	seastar::future<> CqlPasswordAuthenticator::authenticate(
		const seastar::shared_ptr<CqlConnection>& connection,
		const CqlConnectionStream& stream) const {
		// TODO
		throw CqlNotImplementedException(CQL_CODEINFO);
	}
}

