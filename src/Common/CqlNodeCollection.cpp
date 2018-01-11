#include <CqlDriver/Common/CqlNodeCollection.hpp>
#include "CqlNodeCollectionImpl.hpp"

namespace cql {
	/** Create a default implementation of CqlNodeCollection */
	seastar::shared_ptr<CqlNodeCollection> CqlNodeCollection::create(
		const std::vector<CqlNodeConfiguration>& initialNodes) {
		return seastar::make_shared<CqlNodeCollectionImpl>(initialNodes);
	}
}

