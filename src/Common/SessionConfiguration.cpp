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
			defaultConsistency(ConsistencyLevel::Quorum),
			prepareAllQueries(false),
			logger(Logger::createNoop()) { }

		std::size_t minPoolSize;
		std::size_t maxPoolSize;
		std::size_t maxWaitersAfterConnectionsExhausted;
		std::chrono::milliseconds dnsCacheTime;
		std::string defaultKeySpace;
		ConsistencyLevel defaultConsistency;
		bool prepareAllQueries;
		seastar::shared_ptr<Logger> logger;
	};

	/** Set the minimum database connection pool size */
	SessionConfiguration& SessionConfiguration::setMinPoolSize(std::size_t minPoolSize) {
		data_->minPoolSize = minPoolSize;
		return *this;
	}

	/** Set the maximum database connection pool size */
	SessionConfiguration& SessionConfiguration::setMaxPoolSize(std::size_t maxPoolSize) {
		data_->maxPoolSize = maxPoolSize;
		return *this;
	}

	/** Set how many waiters can exist after connections are exhausted */
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

	/** Set the default consistency level */
	SessionConfiguration& SessionConfiguration::setDefaultConsistency(ConsistencyLevel consistency) {
		data_->defaultConsistency = consistency;
		return *this;
	}

	/** Set should prepare all queries by default */
	SessionConfiguration& SessionConfiguration::setPrepareAllQueries(bool value) {
		data_->prepareAllQueries = value;
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

	/** Get the default consistency level */
	ConsistencyLevel SessionConfiguration::getDefaultConsistency() const {
		return data_->defaultConsistency;
	}

	/** Get should prepare all queries by default */
	bool SessionConfiguration::getPrepareAllQueries() const {
		return data_->prepareAllQueries;
	}

	/** Get the logger instance */
	const seastar::shared_ptr<Logger>& SessionConfiguration::getLogger() const& {
		return data_->logger;
	}

	/** Constructor */
	SessionConfiguration::SessionConfiguration() :
		data_(std::make_unique<SessionConfigurationData>()) { }

	/** Destructor */
	SessionConfiguration::~SessionConfiguration() = default;

	/** Copy constructor */
	SessionConfiguration::SessionConfiguration(const SessionConfiguration& other) :
		data_(std::make_unique<SessionConfigurationData>(*other.data_)) { }
	
	/** Move constructor */
	SessionConfiguration::SessionConfiguration(SessionConfiguration&& other) :
		SessionConfiguration(other) {
		// Same as copy, disallow empty pointer
	}

	/** Copy assignment */
	SessionConfiguration& SessionConfiguration::operator=(const SessionConfiguration& other) {
		if (this != &other) {
			data_ = std::make_unique<SessionConfigurationData>(*other.data_);
		}
		return *this;
	}

	/** Move assignment */
	SessionConfiguration& SessionConfiguration::operator=(SessionConfiguration&& other) {
		// Same as copy, disallow empty pointer
		return *this = other;
	}
}

