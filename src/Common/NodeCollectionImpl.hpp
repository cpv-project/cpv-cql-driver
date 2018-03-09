#pragma once
#include <vector>
#include <core/shared_ptr.hh>
#include <core/queue.hh>
#include <CQLDriver/Common/NodeCollection.hpp>

namespace cql {
	/** Default implementation of NodeCollection */
	class NodeCollectionImpl : public NodeCollection {
	public:
		/** Choose a node for the new database connection */
		seastar::lw_shared_ptr<NodeConfiguration> chooseOneNode() override;

		/** Get how many nodes managed by this collection */
		std::size_t getNodesCount() const override;

		/** Report connect to this node has failed */
		void reportFailure(const seastar::lw_shared_ptr<NodeConfiguration>& node) override;

		/** Report connect to this node has been successful */
		void reportSuccess(const seastar::lw_shared_ptr<NodeConfiguration>& node) override;

		/** Constructor */
		explicit NodeCollectionImpl(const std::vector<NodeConfiguration>& initialNodes);

	private:
		struct NodeRecord {
			seastar::lw_shared_ptr<NodeConfiguration> node;
			bool isFault;
			explicit NodeRecord(seastar::lw_shared_ptr<NodeConfiguration>&& node) :
				node(std::move(node)), isFault(false) { }
		};

		/**
		 * Get internal node record.
		 * Return allNodes_.end() if no such record found.
		 */
		std::vector<NodeRecord>::iterator getNodeRecord(
			const seastar::lw_shared_ptr<NodeConfiguration>& node);

	private:
		std::vector<NodeRecord> allNodes_;
		std::size_t nextIndex_;
		seastar::queue<std::size_t> faultNodeIndexes_;
	};
}

