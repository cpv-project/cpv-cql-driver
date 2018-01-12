#pragma once
#include <cstdint>
#include <utility>
#include <chrono>
#include <core/sstring.hh>
#include <net/socket_defs.hh>

namespace cql {
	/**
	 * The configuration of per database server node
	 */
	class CqlNodeConfiguration {
	public:
		/** Set the hostname and the port of this node */
		CqlNodeConfiguration& setAddress(seastar::sstring&& hostname, std::uint16_t port);

		/** Set should connect this node with ssl connection, defaut value is false */
		CqlNodeConfiguration& setUseSsl(bool value);

		/** Set should use frame compression if available, defaut value is false */
		CqlNodeConfiguration& setUseCompression(bool value);

		/** Set how many streams can hold in single connection, default value is 20 */
		CqlNodeConfiguration& setMaxStream(std::size_t value);

		/** Set to use password authentication for this node */
		CqlNodeConfiguration& setPasswordAuthentication(
			seastar::sstring&& username, seastar::sstring&& password);

		/** Get the hostname and the port of this node */
		const std::pair<seastar::sstring, std::uint16_t>& getAddress() const&;

		/** Get should connect this node with ssl connection */
		bool getUseSsl() const;

		/** Get should use frame compression if available */
		bool getUseCompression() const;

		/** Get how many streams can hold in single connection */
		std::size_t getMaxStream() const;

		/** Get the full authentication class name */
		const seastar::sstring& getAuthenticatorClass() const&;

		/** Get the authentication data, the format depends on the class */
		const seastar::sstring& getAuthenticatorData() const&;

		/** Get the resolved ip address, return whether the ip address is available and not expired */
		bool getIpAddress(
			seastar::socket_address& ipAddress,
			std::chrono::milliseconds dnsCacheTime) const;

		/** Update the resolved ip address */
		void updateIpAddress(const seastar::socket_address& ipAddress);

		/** Constructor */
		CqlNodeConfiguration();

	private:
		std::pair<seastar::sstring, std::uint16_t> address_;
		bool useSsl_;
		bool useCompression_;
		std::size_t maxStream_;
		seastar::sstring authenticatorClass_;
		seastar::sstring authenticatorData_;
		seastar::socket_address ipAddress_;
		bool ipAddressIsResolved_;
		bool ipAddressIsFixed_;
		std::chrono::system_clock::time_point ipAddressUpdatedTime_;
	};
}

