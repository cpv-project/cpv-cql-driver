#include <CqlDriver/Common/CqlSessionConfiguration.hpp>

namespace cql {
	/** Set the maximum database connection pool size */
	CqlSessionConfiguration& CqlSessionConfiguration::setMaxPoolSize(std::size_t maxPoolSize) {
		maxPoolSize_ = maxPoolSize;
		return *this;
	}

	/** Set the dns cache time for the hostname of nodes */
	CqlSessionConfiguration& CqlSessionConfiguration::setDnsCacheTime(std::chrono::milliseconds dnsCacheTime) {
		dnsCacheTime_ = dnsCacheTime;
		return *this;
	}

	/** Get the maximum database connection pool size */
	std::size_t CqlSessionConfiguration::getMaxPoolSize() const {
		return maxPoolSize_;
	}

	/** Get the dns cache time for the hostname of nodes */
	std::chrono::milliseconds CqlSessionConfiguration::getDnsCacheTime() const {
		return dnsCacheTime_;
	}

	/** Constructor */
	CqlSessionConfiguration::CqlSessionConfiguration() :
		maxPoolSize_(100),
		dnsCacheTime_(30000) { }
}

