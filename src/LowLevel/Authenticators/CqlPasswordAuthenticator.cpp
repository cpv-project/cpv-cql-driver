#include <CqlDriver/Common/Exceptions/CqlNotImplementedException.hpp>
#include "CqlPasswordAuthenticator.hpp"

namespace cql {
	/** Perform the authentication */
	seastar::future<> CqlPasswordAuthenticator::authenticate(CqlConnection&) const {
		// TODO
		throw CqlNotImplementedException(CQL_CODEINFO);
	}

	/** Constructor */
	CqlPasswordAuthenticator::CqlPasswordAuthenticator(
		const std::string& username, const std::string& password) :
		username_(username),
		password_(password) { }
}

