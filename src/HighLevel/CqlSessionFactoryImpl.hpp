#pragma once
#include <CqlDriver/HighLevel/CqlSessionFactory.hpp>

namespace cql {
	/** Default implementation of CqlSessionFactory */
	class CqlSessionFactoryImpl : public CqlSessionFactory {
	public:
		/** Create a new session instance */
		CqlObject<CqlSession> getSession() override;

		/** Constructor */
		CqlSessionFactoryImpl(
			const CqlSessionConfiguration& sessionConfiguration,
			const seastar::shared_ptr<CqlNodeCollection>& nodeCollection);
	};
}

