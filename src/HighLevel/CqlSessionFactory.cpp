#include <CqlDriver/HighLevel/CqlSessionFactory.hpp>
#include "CqlSessionFactoryImpl.hpp"

namespace cql {
	/** Create a default implementation of CqlSessionFactory */
	seastar::shared_ptr<CqlSessionFactory> CqlSessionFactory::create(
		const CqlSessionConfiguration& sessionConfiguration,
		const seastar::shared_ptr<CqlNodeCollection>& nodeCollection) {
		return seastar::make_shared<CqlSessionFactoryImpl>(sessionConfiguration, nodeCollection);
	}
}

