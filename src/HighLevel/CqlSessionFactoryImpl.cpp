#include <CqlDriver/Common/Exceptions/CqlNotImplementedException.hpp>
#include "CqlSessionFactoryImpl.hpp"

namespace cql {
	/** Create a new session instance */
	std::unique_ptr<CqlSession> CqlSessionFactoryImpl::getSession() {
		throw CqlNotImplementedException(CQL_CODEINFO, "not implemented");
	}

	/** Constructor */
	CqlSessionFactoryImpl::CqlSessionFactoryImpl(
		const CqlSessionConfiguration&,
		const seastar::shared_ptr<CqlNodeCollection>&) {
		throw CqlNotImplementedException(CQL_CODEINFO, "not implemented");
	}
}

