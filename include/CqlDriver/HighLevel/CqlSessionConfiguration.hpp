#pragma once
#include <string>
#include <vector>
#include <net/socket_defs.hh>

namespace cql {
	/**
	 * The all in one configuration
	 */
	class CqlSessionConfiguration {
	public:
		/**
		 * Add database server address to list.
		 * Example(default port): configuration.addServerAddress("127.0.0.1");
		 * Example(custom port): configuration.addServerAddress("127.0.0.1", 9044);
		 * This function only supports ipv4 address, for ipv6 please use the function below.
		 */
		void addServerAddress(const char* ip, std::uint16_t port = 9042);

		/**
		 * Add database server address to list.
		 * Example: configuration.addServerAddress(seastar::make_ipv4_address({ "127.0.0.1", 9042 }))
		 */
		void addServerAddress(seastar::socket_address address);

		/** Clear all database server addresses. */
		void clearServerAddresses();

		/**
		 * Set whether the database connection should use ssl.
		 * Either all database connections use ssl or none,
		 * per server configuration is unsupported (think of auto discover).
		 * The default value is false.
		 */
		void setConnectWithSsl(bool value);

		/**
		 * Set the username for authentication,
		 * empty mean authentication is not required by database server.
		 * The default value is empty.
		 */
		void setUsername(std::string username);

		/** Set the password for authentication. */
		void setPassword(std::string password);

		/**
		 * Set the maximum database connection pool size.
		 * Notice it's per core configuration,
		 * for example if the max pool size is 100 and there 2 cpu cores,
		 * then the maximum database connections in the pool for single process is 200.
		 * The default value is 100.
		 */
		void setMaxPoolSize(std::size_t size);

		/**
		 * Set whether to discover new server addresses by the events sent from database server.
		 * If not set, only the initial server addresses will used.
		 * The default value is false.
		 */
		void setAutoDiscoverServerAddress(bool value);

		/** Get the database server address list. */
		const std::vector<seastar::socket_address>& getServerAddresses() const&;

		/** Get whether the database connection should use ssl. */
		bool getConnectWithSsl() const;

		/** Get the username for authentication, */
		const std::string& getUsername() const&;

		/** Get the password for authentication. */
		const std::string& getPassword() const&;

		/** Get the maximum database connection pool size. */
		std::size_t getMaxPoolSize() const;

		/** Get whether to discover new server addresses by the events sent from database server. */
		bool getAutoDiscoverServerAddresses() const;

		/** Constructor */
		CqlSessionConfiguration();

	private:
		std::vector<seastar::socket_address> serverAddresses_;
		bool connectWithSsl_;
		std::string username_;
		std::string password_;
		std::size_t maxPoolSize_;
		bool autoDiscoverServerAddresses_;
	};
}

