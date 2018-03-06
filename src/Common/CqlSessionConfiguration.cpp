#include <CqlDriver/Common/CqlSessionConfiguration.hpp>

namespace cql {
	/** Defines members of CqlSessionConfiguration */
	class CqlSessionConfigurationData {
	public:
		CqlSessionConfigurationData() :
			minPoolSize(5),
			maxPoolSize(100),
			maxWaitersAfterConnectionsExhausted(100),
			dnsCacheTime(30000) { }

		std::size_t minPoolSize;
		std::size_t maxPoolSize;
		std::size_t maxWaitersAfterConnectionsExhausted;
		std::chrono::milliseconds dnsCacheTime;
	};

	/** Set the minimum database connection pool size.  */
	CqlSessionConfiguration& CqlSessionConfiguration::setMinPoolSize(std::size_t minPoolSize) {
		data_->minPoolSize = minPoolSize;
		return *this;
	}

	/** Set the maximum database connection pool size */
	CqlSessionConfiguration& CqlSessionConfiguration::setMaxPoolSize(std::size_t maxPoolSize) {
		data_->maxPoolSize = maxPoolSize;
		return *this;
	}

	/** Set how many waitiers can exist after connections are exhausted */
	CqlSessionConfiguration& CqlSessionConfiguration::setMaxWaitersAfterConnectionsExhausted(std::size_t value) {
		data_->maxWaitersAfterConnectionsExhausted = value;
		return *this;
	}

	/** Set the dns cache time for the hostname of nodes */
	CqlSessionConfiguration& CqlSessionConfiguration::setDnsCacheTime(std::chrono::milliseconds dnsCacheTime) {
		data_->dnsCacheTime = dnsCacheTime;
		return *this;
	}

	/** Get the minimum database connection pool size */
	std::size_t CqlSessionConfiguration::getMinPoolSize() const {
		return data_->minPoolSize;
	}

	/** Get the maximum database connection pool size */
	std::size_t CqlSessionConfiguration::getMaxPoolSize() const {
		return data_->maxPoolSize;
	}

	/** Get how many waitiers can exist after connections are exhausted */
	std::size_t CqlSessionConfiguration::getMaxWaitersAfterConnectionsExhausted() const {
		return data_->maxWaitersAfterConnectionsExhausted;
	}

	/** Get the dns cache time for the hostname of nodes */
	std::chrono::milliseconds CqlSessionConfiguration::getDnsCacheTime() const {
		return data_->dnsCacheTime;
	}

	/** Constructor */
	CqlSessionConfiguration::CqlSessionConfiguration() :
		data_(seastar::make_shared<CqlSessionConfigurationData>()) { }
}

