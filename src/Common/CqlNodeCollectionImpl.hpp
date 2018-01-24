#pragma once
#include <CqlDriver/Common/CqlNodeCollection.hpp>

namespace cql {
	/** Default implementation of CqlNodeCollection */
	class CqlNodeCollectionImpl : public CqlNodeCollection {
	public:
		/** Choose a node for the new database connection */
		seastar::lw_shared_ptr<CqlNodeConfiguration> chooseOneNode() override;
		
		/** Report connect to this node has failed */
		void reportFailure(const seastar::shared_ptr<CqlNodeConfiguration>& node) override;

		/** Report connect to this node has been successful */
		void reportSuccess(const seastar::shared_ptr<CqlNodeConfiguration>& node) override;

		/** Constructor */
		explicit CqlNodeCollectionImpl(const std::vector<CqlNodeConfiguration>& initialNodes);
	};
}

