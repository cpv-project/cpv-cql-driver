#include <CQLDriver/Common/SessionConfiguration.hpp>

namespace cql {
	/** Defines members of SessionConfiguration */
	class SessionConfigurationData {
	public:
		SessionConfigurationData() :
			minPoolSize(5),
			maxPoolSize(100),
			maxWaitersAfterConnectionsExhausted(100),
			dnsCacheTime(30000),
			defaultKeySpace() { }

		std::size_t minPoolSize;
		std::size_t maxPoolSize;
		std::size_t maxWaitersAfterConnectionsExhausted;
		std::chrono::milliseconds dnsCacheTime;
		std::string defaultKeySpace;
	};

	/** Set the minimum database connection pool size.  */
	SessionConfiguration& SessionConfiguration::setMinPoolSize(std::size_t minPoolSize) {
		data_->minPoolSize = minPoolSize;
		return *this;
	}

	/** Set the maximum database connection pool size */
	SessionConfiguration& SessionConfiguration::setMaxPoolSize(std::size_t maxPoolSize) {
		data_->maxPoolSize = maxPoolSize;
		return *this;
	}

	/** Set how many waitiers can exist after connections are exhausted */
	SessionConfiguration& SessionConfiguration::setMaxWaitersAfterConnectionsExhausted(std::size_t value) {
		data_->maxWaitersAfterConnectionsExhausted = value;
		return *this;
	}

	/** Set the dns cache time for the hostname of nodes */
	SessionConfiguration& SessionConfiguration::setDnsCacheTime(std::chrono::milliseconds dnsCacheTime) {
		data_->dnsCacheTime = dnsCacheTime;
		return *this;
	}

	/** Set the default keyspace of all connections created by this configuration */
	SessionConfiguration& SessionConfiguration::setDefaultKeySpace(const std::string& keySpace) {
		data_->defaultKeySpace = keySpace;
		return *this;
	}

	/** Get the minimum database connection pool size */
	std::size_t SessionConfiguration::getMinPoolSize() const {
		return data_->minPoolSize;
	}

	/** Get the maximum database connection pool size */
	std::size_t SessionConfiguration::getMaxPoolSize() const {
		return data_->maxPoolSize;
	}

	/** Get how many waitiers can exist after connections are exhausted */
	std::size_t SessionConfiguration::getMaxWaitersAfterConnectionsExhausted() const {
		return data_->maxWaitersAfterConnectionsExhausted;
	}

	/** Get the dns cache time for the hostname of nodes */
	std::chrono::milliseconds SessionConfiguration::getDnsCacheTime() const {
		return data_->dnsCacheTime;
	}

	/** Get the default keyspace of all connections created by this configuration */
	const std::string& SessionConfiguration::getDefaultKeySpace() const& {
		return data_->defaultKeySpace;
	}

	/** Constructor */
	SessionConfiguration::SessionConfiguration() :
		data_(seastar::make_shared<SessionConfigurationData>()) { }
}

