#include <algorithm>
#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include "CqlNodeCollectionImpl.hpp"

namespace cql {
	/** Choose a node for the new database connection */
	seastar::lw_shared_ptr<CqlNodeConfiguration> CqlNodeCollectionImpl::chooseOneNode() {
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
		throw CqlLogicException(CQL_CODEINFO, "choose one node failed");
	}

	/** Get how many nodes managed by this collection */
	std::size_t CqlNodeCollectionImpl::getNodesCount() const {
		return allNodes_.size();
	}

	/** Report connect to this node has failed */
	void CqlNodeCollectionImpl::reportFailure(const seastar::lw_shared_ptr<CqlNodeConfiguration>& node) {
		auto nodeRecord = getNodeRecord(node);
		if (nodeRecord == allNodes_.end()) {
			throw CqlLogicException(CQL_CODEINFO, "node does not exist in this collection");
		}
		if (!nodeRecord->isFault) {
			nodeRecord->isFault = true;
			std::size_t index = nodeRecord - allNodes_.begin();
			faultNodeIndexes_.push(+index);
		}
	}

	/** Report connect to this node has been successful */
	void CqlNodeCollectionImpl::reportSuccess(const seastar::lw_shared_ptr<CqlNodeConfiguration>& node) {
		auto nodeRecord = getNodeRecord(node);
		if (nodeRecord == allNodes_.end()) {
			throw CqlLogicException(CQL_CODEINFO, "node does not exist in this collection");
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
	CqlNodeCollectionImpl::CqlNodeCollectionImpl(const std::vector<CqlNodeConfiguration>& initialNodes) :
		allNodes_(),
		nextIndex_(0),
		faultNodeIndexes_(initialNodes.size()) {
		// prepare nodes array and sort by the pointer address
		if (initialNodes.empty()) {
			throw CqlLogicException(CQL_CODEINFO, "initial nodes can't be empty");
		}
		for (const auto& node : initialNodes) {
			allNodes_.emplace_back(NodeRecord({
				seastar::make_lw_shared<CqlNodeConfiguration>(node),
				false
			}));
		}
		std::sort(allNodes_.begin(), allNodes_.end(), [](auto& a, auto& b) {
			return a.node.get() < b.node.get();
		});
	}

	/** Get internal node record */
	std::vector<CqlNodeCollectionImpl::NodeRecord>::iterator CqlNodeCollectionImpl::getNodeRecord(
		const seastar::lw_shared_ptr<CqlNodeConfiguration>& node) {
		auto it = std::lower_bound(allNodes_.begin(), allNodes_.end(), node, [](auto& a, auto& b) {
			return a.node.get() < b.get();
		});
		if (it != allNodes_.end() && it->node.get() != node.get()) {
			it = allNodes_.end();
		}
		return it;
	}
}

