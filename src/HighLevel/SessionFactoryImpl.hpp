#pragma once
#include <CQLDriver/HighLevel/SessionFactory.hpp>

namespace cql {
	/** Default implementation of SessionFactory */
	class SessionFactoryImpl : public SessionFactory {
	public:
		/** Create a new session instance */
		Object<Session> getSession() override;

		/** Constructor */
		SessionFactoryImpl(
			const SessionConfiguration& sessionConfiguration,
			const seastar::shared_ptr<NodeCollection>& nodeCollection);
	};
}

