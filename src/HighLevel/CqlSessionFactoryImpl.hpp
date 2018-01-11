#pragma once
#include <CqlDriver/HighLevel/CqlSessionFactory.hpp>

namespace cql {
	/** Default implementation of CqlSessionFactory */
	class CqlSessionFactoryImpl : public CqlSessionFactory {
	public:
		/** Create a new session instance */
		std::unique_ptr<CqlSession> getSession() override;

		/** Constructor */
		CqlSessionFactoryImpl(
			const CqlSessionConfiguration& sessionConfiguration,
			const seastar::shared_ptr<CqlNodeCollection>& nodeCollection);
	};
}

