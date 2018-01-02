#include <CqlDriver/HighLevel/CqlSessionConfiguration.hpp>
#include "CqlSessionConfigurationImpl.hpp"
#include "../LowLevel/Connectors/CqlSslConnector.hpp"
#include "../LowLevel/Authenticators/CqlPasswordAuthenticator.hpp"

namespace cql {
	/** Add database server address to list. */
	void CqlSessionConfiguration::addServerAddress(const char* ip, std::uint16_t port) {
		addServerAddress(seastar::make_ipv4_address({ ip, port }));
	}

	/** Add database server address to list. */
	void CqlSessionConfiguration::addServerAddress(seastar::socket_address address) {
		impl_->serverAddresses.emplace_back(address);
	}

	/** Connect database with SSL and verify with the given pem certificate. */
	void CqlSessionConfiguration::connectWithSsl(const std::string& pem) {
		impl_->connector = seastar::make_shared<CqlSslConnector>(pem);
	}

	/** Set the username and password for authentication. */
	void CqlSessionConfiguration::authenticateWithPassword(
		const std::string& username, const std::string& password) {
		impl_->authenticator = seastar::make_shared<CqlPasswordAuthenticator>(username, password);
	}

	/** Set the maximum database connection pool size. */
	void CqlSessionConfiguration::setMaxPoolSize(std::size_t size) {
		impl_->maxPoolSize = size;
	}

	/** Set whether to discover new server addresses by the events sent from database server. */
	void CqlSessionConfiguration::setAutoDiscoverServerAddress(bool value) {
		impl_->autoDiscoverServerAddress = value;
	}

	/** Get the implementation pointer, only for internal usage */
	const std::unique_ptr<CqlSessionConfigurationImpl>& CqlSessionConfiguration::getImpl() const& {
		return impl_;
	}

	/** Constructor */
	CqlSessionConfiguration::CqlSessionConfiguration() :
		impl_(std::make_unique<CqlSessionConfigurationImpl>()) { }
}

