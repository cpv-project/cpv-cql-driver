#include <CQLDriver/Common/Exceptions/NotImplementedException.hpp>
#include "./SessionFactoryImpl.hpp"

namespace cql {
	/** Create a new session instance */
	Object<Session> SessionFactoryImpl::getSession() {
		throw NotImplementedException(CQL_CODEINFO, "not implemented");
	}

	/** Constructor */
	SessionFactoryImpl::SessionFactoryImpl(
		const SessionConfiguration&,
		const seastar::shared_ptr<NodeCollection>&) {
		throw NotImplementedException(CQL_CODEINFO, "not implemented");
	}
}

