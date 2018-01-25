#include <CqlDriver/Common/CqlSessionConfiguration.hpp>

namespace cql {
	/** Set the minimum database connection pool size.  */
	CqlSessionConfiguration& CqlSessionConfiguration::setMinPoolSize(std::size_t minPoolSize) {
		minPoolSize_ = minPoolSize;
		return *this;
	}

	/** Set the maximum database connection pool size */
	CqlSessionConfiguration& CqlSessionConfiguration::setMaxPoolSize(std::size_t maxPoolSize) {
		maxPoolSize_ = maxPoolSize;
		return *this;
	}

	/** Set how many waitiers can exist after connections are exhausted */
	CqlSessionConfiguration& CqlSessionConfiguration::setMaxWaitersAfterConnectionsExhausted(std::size_t value) {
		maxWaitersAfterConnectionsExhausted_ = value;
		return *this;
	}

	/** Set the dns cache time for the hostname of nodes */
	CqlSessionConfiguration& CqlSessionConfiguration::setDnsCacheTime(std::chrono::milliseconds dnsCacheTime) {
		dnsCacheTime_ = dnsCacheTime;
		return *this;
	}

	/** Get the minimum database connection pool size */
	std::size_t CqlSessionConfiguration::getMinPoolSize() const {
		return minPoolSize_;
	}

	/** Get the maximum database connection pool size */
	std::size_t CqlSessionConfiguration::getMaxPoolSize() const {
		return maxPoolSize_;
	}

	/** Get how many waitiers can exist after connections are exhausted */
	std::size_t CqlSessionConfiguration::getMaxWaitersAfterConnectionsExhausted() const {
		return maxWaitersAfterConnectionsExhausted_;
	}

	/** Get the dns cache time for the hostname of nodes */
	std::chrono::milliseconds CqlSessionConfiguration::getDnsCacheTime() const {
		return dnsCacheTime_;
	}

	/** Constructor */
	CqlSessionConfiguration::CqlSessionConfiguration() :
		minPoolSize_(5),
		maxPoolSize_(100),
		maxWaitersAfterConnectionsExhausted_(100),
		dnsCacheTime_(30000) { }
}

