#include <net/api.hh>
#include <CqlDriver/HighLevel/CqlSessionConfiguration.hpp>

namespace cql {
	/** Add database server address to list. */
	void CqlSessionConfiguration::addServerAddress(const char* ip, std::uint16_t port) {
		addServerAddress(seastar::make_ipv4_address({ ip, port }));
	}

	/** Add database server address to list. */
	void CqlSessionConfiguration::addServerAddress(seastar::socket_address address) {
		serverAddresses_.emplace_back(address);
	}

	/** Clear all database server addresses. */
	void CqlSessionConfiguration::clearServerAddresses() {
		serverAddresses_.clear();
	}

	/** Set whether the database connection should use ssl. */
	void CqlSessionConfiguration::setConnectWithSsl(bool value) {
		connectWithSsl_ = value;
	}

	/** Set the password for authentication. */
	void CqlSessionConfiguration::setUsername(std::string username) {
		username_ = username;
	}

	/** Set the password for authentication. */
	void CqlSessionConfiguration::setPassword(std::string password) {
		password_ = password;
	}

	/** Set the maximum database connection pool size. */
	void CqlSessionConfiguration::setMaxPoolSize(std::size_t size) {
		maxPoolSize_ = size;
	}

	/** Set whether to discover new server addresses by the events sent from database server. */
	void CqlSessionConfiguration::setAutoDiscoverServerAddress(bool value) {
		autoDiscoverServerAddresses_ = value;
	}

	/** Get the database server address list. */
	const std::vector<seastar::socket_address>& CqlSessionConfiguration::getServerAddresses() const& {
		return serverAddresses_;
	}

	/** Get whether the database connection should use ssl. */
	bool CqlSessionConfiguration::getConnectWithSsl() const {
		return connectWithSsl_;
	}

	/** Get the username for authentication, */
	const std::string& CqlSessionConfiguration::getUsername() const& {
		return username_;
	}

	/** Get the password for authentication. */
	const std::string& CqlSessionConfiguration::getPassword() const& {
		return password_;
	}

	/** Get the maximum database connection pool size. */
	std::size_t CqlSessionConfiguration::getMaxPoolSize() const {
		return maxPoolSize_;
	}

	/** Get whether to discover new server addresses by the events sent from database server. */
	bool CqlSessionConfiguration::getAutoDiscoverServerAddresses() const {
		return autoDiscoverServerAddresses_;
	}

	/** Constructor */
	CqlSessionConfiguration::CqlSessionConfiguration() :
		serverAddresses_(),
		connectWithSsl_(false),
		username_(),
		password_(),
		maxPoolSize_(100),
		autoDiscoverServerAddresses_(false) { }
}

