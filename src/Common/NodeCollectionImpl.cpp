#include <algorithm>
#include <CQLDriver/Common/Exceptions/LogicException.hpp>
#include <CQLDriver/Common/CommonDefinitions.hpp>
#include "./NodeCollectionImpl.hpp"

namespace cql {
	/** Choose a node for the new database connection */
	seastar::lw_shared_ptr<NodeConfiguration> NodeCollectionImpl::chooseOneNode() {
		// enumerate nodes from last index
		for (std::size_t i = 0, j = allNodes_.size(), k = j+1; i < k; ++i) {
			// try to recover one fault node after each times all nodes has been enumerated
			if (nextIndex_ >= j) {
				nextIndex_ = 0;
				if (!faultNodeIndexes_.empty()) {
					std::size_t faultNodeIndex = faultNodeIndexes_.pop();
					faultNodeIndexes_.push(+faultNodeIndex);
					return allNodes_.at(faultNodeIndex).node;
				}
			}
			// pick a node that not marked as fault
			auto& nodeRecord = allNodes_[nextIndex_++];
			if (!nodeRecord.isFault) {
				return nodeRecord.node;
			}
		}
		// here should be unreachable
		throw LogicException(CQL_CODEINFO, "choose one node failed");
	}

	/** Get how many nodes managed by this collection */
	std::size_t NodeCollectionImpl::getNodesCount() const {
		return allNodes_.size();
	}

	/** Report connect to this node has failed */
	void NodeCollectionImpl::reportFailure(const seastar::lw_shared_ptr<NodeConfiguration>& node) {
		auto nodeRecord = getNodeRecord(node);
		if (CQL_UNLIKELY(nodeRecord == allNodes_.end())) {
			throw LogicException(CQL_CODEINFO, "node does not exist in this collection");
		}
		if (!nodeRecord->isFault) {
			nodeRecord->isFault = true;
			std::size_t index = nodeRecord - allNodes_.begin();
			faultNodeIndexes_.push(+index);
		}
	}

	/** Report connect to this node has been successful */
	void NodeCollectionImpl::reportSuccess(const seastar::lw_shared_ptr<NodeConfiguration>& node) {
		auto nodeRecord = getNodeRecord(node);
		if (CQL_UNLIKELY(nodeRecord == allNodes_.end())) {
			throw LogicException(CQL_CODEINFO, "node does not exist in this collection");
		}
		if (nodeRecord->isFault) {
			nodeRecord->isFault = false;
			std::size_t index = nodeRecord - allNodes_.begin();
			for (std::size_t i = 0, j = faultNodeIndexes_.size(); i < j; ++i) {
				std::size_t faultNodeIndex = faultNodeIndexes_.pop();
				if (faultNodeIndex != index) {
					faultNodeIndexes_.push(+faultNodeIndex);
				}
			}
		}
	}

	/** Constructor */
	NodeCollectionImpl::NodeCollectionImpl(const std::vector<NodeConfiguration>& initialNodes) :
		allNodes_(),
		nextIndex_(0),
		faultNodeIndexes_(initialNodes.size()) {
		// prepare nodes array and sort by the pointer address
		if (CQL_UNLIKELY(initialNodes.empty())) {
			throw LogicException(CQL_CODEINFO, "initial nodes can't be empty");
		}
		for (const auto& node : initialNodes) {
			allNodes_.emplace_back(seastar::make_lw_shared<NodeConfiguration>(node));
		}
		std::sort(allNodes_.begin(), allNodes_.end(), [](auto& a, auto& b) {
			return a.node.get() < b.node.get();
		});
	}

	/** Get internal node record */
	std::vector<NodeCollectionImpl::NodeRecord>::iterator NodeCollectionImpl::getNodeRecord(
		const seastar::lw_shared_ptr<NodeConfiguration>& node) {
		auto it = std::lower_bound(allNodes_.begin(), allNodes_.end(), node, [](auto& a, auto& b) {
			return a.node.get() < b.get();
		});
		if (it != allNodes_.end() && it->node.get() != node.get()) {
			it = allNodes_.end();
		}
		return it;
	}
}

