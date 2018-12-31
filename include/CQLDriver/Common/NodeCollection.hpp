#pragma once
#include <vector>
#include <seastar/core/shared_ptr.hh>
#include "./NodeConfiguration.hpp"

namespace cql {
	/** Interface use to manage node configurations and choose node based on some strategy */
	class NodeCollection {
	public:
		/** Choose a node for the new database connection */
		virtual seastar::lw_shared_ptr<NodeConfiguration> chooseOneNode() = 0;

		/** Get how many nodes managed by this collection */
		virtual std::size_t getNodesCount() const = 0;

		/** Report connect to this node has failed */
		virtual void reportFailure(const seastar::lw_shared_ptr<NodeConfiguration>& node) = 0;

		/** Report connect to this node has been successful */
		virtual void reportSuccess(const seastar::lw_shared_ptr<NodeConfiguration>& node) = 0;

		/** Virtual destructor */
		virtual ~NodeCollection() = default;

		/** Create a default implementation of NodeCollection */
		static seastar::shared_ptr<NodeCollection> create(
			const std::vector<NodeConfiguration>& initialNodes);
	};
}

