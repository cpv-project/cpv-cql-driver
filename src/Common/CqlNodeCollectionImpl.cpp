#include <CqlDriver/Common/Exceptions/CqlNotImplementedException.hpp>
#include "CqlNodeCollectionImpl.hpp"

namespace cql {
	/** Choose a node for the new database connection */
	seastar::lw_shared_ptr<CqlNodeConfiguration> CqlNodeCollectionImpl::chooseOneNode() {
		throw cql::CqlNotImplementedException(CQL_CODEINFO, "not implemented");
	}
	
	/** Report connect to this node has failed */
	void CqlNodeCollectionImpl::reportFailure(const seastar::shared_ptr<CqlNodeConfiguration>& node) {
		throw cql::CqlNotImplementedException(CQL_CODEINFO, "not implemented");
	}

	/** Report connect to this node has been successful */
	void CqlNodeCollectionImpl::reportSuccess(const seastar::shared_ptr<CqlNodeConfiguration>& node) {
		throw cql::CqlNotImplementedException(CQL_CODEINFO, "not implemented");
	}

	/** Constructor */
	CqlNodeCollectionImpl::CqlNodeCollectionImpl(const std::vector<CqlNodeConfiguration>&) {
		throw cql::CqlNotImplementedException(CQL_CODEINFO, "not implemented");
	}
}

