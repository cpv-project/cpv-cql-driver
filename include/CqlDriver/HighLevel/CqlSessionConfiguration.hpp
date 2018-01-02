#pragma once
#include <string>
#include <vector>
#include <memory>
#include <net/socket_defs.hh>

namespace cql {
	class CqlSessionConfigurationImpl;

	/** High level cql database client configuration */
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

		/**
		 * Connect database with SSL and verify with the given pem certificate.
		 * Either all database connections use ssl or none,
		 * per server configuration is unsupported (think of auto discover).
		 * If this function is not called then the connection will use plain tcp.
		 */
		void connectWithSsl(const std::string& pem);

		/**
		 * Set the username and password for authentication.
		 * If this function is not called then no password authentication will perform.
		 */
		void authenticateWithPassword(const std::string& username, const std::string& password);

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

		/** Get the implementation pointer, only for internal usage */
		const std::unique_ptr<CqlSessionConfigurationImpl>& getImpl() const&;

		/** Constructor */
		CqlSessionConfiguration();

	private:
		std::unique_ptr<CqlSessionConfigurationImpl> impl_;
	};
}

