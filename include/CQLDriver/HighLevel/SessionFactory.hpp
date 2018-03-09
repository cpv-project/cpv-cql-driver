#pragma once
#include <core/shared_ptr.hh>
#include "../Common/Utility/Object.hpp"
#include "../Common/SessionConfiguration.hpp"
#include "../Common/NodeCollection.hpp"
#include "./Session.hpp"

namespace cql {
	/** Interface use to create session instances */
	class SessionFactory {
	public:
		/** Create a new session instance */
		virtual Object<Session> getSession() = 0;

		/** Virtual destructor */
		virtual ~SessionFactory() = default;

		/** Create a default implementation of SessionFactory */
		static seastar::shared_ptr<SessionFactory> create(
			const SessionConfiguration& sessionConfiguration,
			const seastar::shared_ptr<NodeCollection>& nodeCollection);
	};
}

