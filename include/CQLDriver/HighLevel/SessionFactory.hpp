#pragma once
#include <seastar/core/shared_ptr.hh>
#include "../Common/SessionConfiguration.hpp"
#include "../Common/NodeCollection.hpp"
#include "./Session.hpp"

namespace cql {
	/** Defines members of SessionFactory */
	class SessionFactoryData;

	/**
	 * Factory class use to create session instances.
	 * It doesn't support cross cpu use, please create a factory for each cpu core.
	 */
	class SessionFactory {
	public:
		/** Create a new session instance */
		Session getSession();

		/** Constructor */
		SessionFactory(
			const SessionConfiguration& sessionConfiguration,
			const seastar::shared_ptr<NodeCollection>& nodeCollection);

	private:
		seastar::shared_ptr<SessionFactoryData> data_;
	};
}

