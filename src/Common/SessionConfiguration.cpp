#include <CQLDriver/Common/SessionConfiguration.hpp>
#include <CQLDriver/Common/Exceptions/LogicException.hpp>

namespace cql {
	/** Defines members of SessionConfiguration */
	class SessionConfigurationData {
	public:
		SessionConfigurationData() :
			minPoolSize(5),
			maxPoolSize(100),
			maxWaitersAfterConnectionsExhausted(100),
			dnsCacheTime(30000),
			defaultKeySpace(),
			logger(Logger::createConsole(LogLevel::Debug)) { }

		std::size_t minPoolSize;
		std::size_t maxPoolSize;
		std::size_t maxWaitersAfterConnectionsExhausted;
		std::chrono::milliseconds dnsCacheTime;
		std::string defaultKeySpace;
		seastar::shared_ptr<Logger> logger;
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

	/** Set the logger instance */
	SessionConfiguration& SessionConfiguration::setLogger(const seastar::shared_ptr<Logger>& logger) {
		if (logger == nullptr) {
			throw LogicException(CQL_CODEINFO, "logger is nullptr");
		}
		data_->logger = logger;
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

	/** Get the logger instance */
	const seastar::shared_ptr<Logger>& SessionConfiguration::getLogger() const& {
		return data_->logger;
	}

	/** Constructor */
	SessionConfiguration::SessionConfiguration() :
		data_(seastar::make_shared<SessionConfigurationData>()) { }
}

