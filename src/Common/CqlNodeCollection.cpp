#include <CqlDriver/Common/Exceptions/CqlNotImplementedException.hpp>
#include <CqlDriver/Common/CqlNodeCollection.hpp>
#include "CqlNodeCollectionImpl.hpp"

namespace cql {
	/** Create a default implementation of CqlNodeCollection */
	seastar::shared_ptr<CqlNodeCollection> CqlNodeCollection::create(
		std::vector<CqlNodeConfiguration>&& initialNodes) {
		throw CqlNotImplementedException(CQL_CODEINFO, "not implemented");
	}
}

