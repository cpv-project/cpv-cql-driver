#include <CQLDriver/Common/NodeCollection.hpp>
#include "./NodeCollectionImpl.hpp"

namespace cql {
	/** Create a default implementation of NodeCollection */
	seastar::shared_ptr<NodeCollection> NodeCollection::create(
		const std::vector<NodeConfiguration>& initialNodes) {
		return seastar::make_shared<NodeCollectionImpl>(initialNodes);
	}
}

