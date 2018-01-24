#pragma once
#include <vector>
#include <core/shared_ptr.hh>
#include <core/queue.hh>
#include <CqlDriver/Common/CqlNodeCollection.hpp>

namespace cql {
	/** Default implementation of CqlNodeCollection */
	class CqlNodeCollectionImpl : public CqlNodeCollection {
	public:
		/** Choose a node for the new database connection */
		seastar::lw_shared_ptr<CqlNodeConfiguration> chooseOneNode() override;

		/** Get how many nodes managed by this collection */
		std::size_t getNodesCount() const override;

		/** Report connect to this node has failed */
		void reportFailure(const seastar::lw_shared_ptr<CqlNodeConfiguration>& node) override;

		/** Report connect to this node has been successful */
		void reportSuccess(const seastar::lw_shared_ptr<CqlNodeConfiguration>& node) override;

		/** Constructor */
		explicit CqlNodeCollectionImpl(const std::vector<CqlNodeConfiguration>& initialNodes);

	private:
		struct NodeRecord {
			seastar::lw_shared_ptr<CqlNodeConfiguration> node;
			bool isFault;
		};

		/**
		 * Get internal node record.
		 * Return allNodes_.end() if no such record found.
		 */
		std::vector<NodeRecord>::iterator getNodeRecord(
			const seastar::lw_shared_ptr<CqlNodeConfiguration>& node);

	private:
		std::vector<NodeRecord> allNodes_;
		std::size_t nextIndex_;
		seastar::queue<std::size_t> faultNodeIndexes_;
	};
}

