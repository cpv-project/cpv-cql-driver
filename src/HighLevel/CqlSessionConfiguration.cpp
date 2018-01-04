#include <CqlDriver/HighLevel/CqlSessionConfiguration.hpp>
#include "CqlSessionConfigurationData.hpp"
#include "../LowLevel/Connectors/CqlSslConnector.hpp"
#include "../LowLevel/Authenticators/CqlPasswordAuthenticator.hpp"

namespace cql {
	/** Add database server address to list. */
	void CqlSessionConfiguration::addServerAddress(const char* ip, std::uint16_t port) {
		addServerAddress(seastar::make_ipv4_address({ ip, port }));
	}

	/** Add database server address to list. */
	void CqlSessionConfiguration::addServerAddress(seastar::socket_address address) {
		data_->serverAddresses.emplace_back(address);
	}

	/** Connect database with SSL and verify with the given pem certificate. */
	void CqlSessionConfiguration::connectWithSsl() {
		data_->connector = seastar::make_shared<CqlSslConnector>();
	}

	/** Set the username and password for authentication. */
	void CqlSessionConfiguration::authenticateWithPassword(
		const std::string& username, const std::string& password) {
		data_->authenticator = seastar::make_shared<CqlPasswordAuthenticator>(username, password);
	}

	/** Set the maximum database connection pool size. */
	void CqlSessionConfiguration::setMaxPoolSize(std::size_t size) {
		data_->maxPoolSize = size;
	}

	/** Set whether to discover new server addresses by the events sent from database server. */
	void CqlSessionConfiguration::setAutoDiscoverServerAddress(bool value) {
		data_->autoDiscoverServerAddress = value;
	}

	/** Get the data pointer, only for internal usage */
	const std::unique_ptr<CqlSessionConfigurationData>& CqlSessionConfiguration::getData() const& {
		return data_;
	}

	/** Constructor */
	CqlSessionConfiguration::CqlSessionConfiguration() :
		data_(std::make_unique<CqlSessionConfigurationData>()) { }
}

