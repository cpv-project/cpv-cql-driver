#pragma once
#include <core/shared_ptr.hh>
#include "../Common/CqlSessionConfiguration.hpp"
#include "../Common/CqlNodeCollection.hpp"
#include "CqlSession.hpp"

namespace cql {
	/** Interface use to create session instances */
	class CqlSessionFactory {
	public:
		/** Create a new session instance */
		virtual std::unique_ptr<CqlSession> getSession() = 0;

		/** Virtual destructor */
		virtual ~CqlSessionFactory() = default;

		/** Create a default implementation of CqlSessionFactory */
		static seastar::shared_ptr<CqlSessionFactory> create(
			const CqlSessionConfiguration& sessionConfiguration,
			const seastar::shared_ptr<CqlNodeCollection>& nodeCollection);
	};
}

