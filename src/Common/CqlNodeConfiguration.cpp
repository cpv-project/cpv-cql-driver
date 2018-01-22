#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include <CqlDriver/Common/Exceptions/CqlNotImplementedException.hpp>
#include <CqlDriver/Common/Exceptions/CqlFormatException.hpp>
#include <CqlDriver/Common/CqlCommonDefinitions.hpp>
#include <CqlDriver/Common/CqlNodeConfiguration.hpp>
#include <stdexcept>
#include <net/inet_address.hh>

namespace cql {
	/** Set the hostname and the port of this node */
	CqlNodeConfiguration& CqlNodeConfiguration::setAddress(
		seastar::sstring&& hostname, std::uint16_t port) {
		address_ = std::make_pair(hostname, port);
		try {
			seastar::net::inet_address inetAddress(hostname);
			if (inetAddress.in_family() == seastar::net::inet_address::family::INET) {
				ipAddress_ = seastar::socket_address(seastar::ipv4_addr(inetAddress, port));
				ipAddressIsResolved_ = true;
				ipAddressIsFixed_ = true;
			} else {
				// seastar's socket_address only support ipv4 now so throw a exception for ipv6
				throw CqlNotImplementedException(CQL_CODEINFO, "ipv6 address isn't supported yet");
			}
		} catch (const std::invalid_argument&) {
			ipAddressIsResolved_ = false;
			ipAddressIsFixed_ = false;
			ipAddressUpdatedTime_ = {};
		}
		return *this;
	}

	/** Set should connect this node with ssl connection */
	CqlNodeConfiguration& CqlNodeConfiguration::setUseSsl(bool value) {
		useSsl_ = value;
		return *this;
	}

	/** Set should use frame compression if available */
	CqlNodeConfiguration& CqlNodeConfiguration::setUseCompression(bool value) {
		useCompression_ = value;
		return *this;
	}

	/** Set how many streams can hold in single connection */
	CqlNodeConfiguration& CqlNodeConfiguration::setMaxStreams(std::size_t value) {
		if (!(value >= 2 && value <= 255)) {
			throw CqlFormatException(CQL_CODEINFO,
				"invalid max streams value, it should >= 2 and <= 255");
		}
		maxStreams_ = value;
		return *this;
	}

	/** Set how many messages can hold in a received queue for single stream */
	CqlNodeConfiguration& CqlNodeConfiguration::setMaxPendingMessages(std::size_t value) {
		if (!(value >= 1)) {
			throw CqlFormatException(CQL_CODEINFO,
				"invalid max pending messages value, it should >= 1");
		}
		maxPendingMessages_ = value;
		return *this;
	}

	/** Set to use password authentication for this node */
	CqlNodeConfiguration& CqlNodeConfiguration::setPasswordAuthentication(
		seastar::sstring&& username, seastar::sstring&& password) {
		authenticatorClass_ = CqlAuthenticatorClasses::PasswordAuthenticator;
		authenticatorData_.resize(0);
		authenticatorData_.append(username.c_str(), username.size());
		authenticatorData_.append("\x00", 1);
		authenticatorData_.append(password.c_str(), password.size());
		return *this;
	}

	/** Get the hostname and the port of this node */
	const std::pair<seastar::sstring, std::uint16_t>& CqlNodeConfiguration::getAddress() const& {
		return address_;
	}

	/** Get should connect this node with ssl connection */
	bool CqlNodeConfiguration::getUseSsl() const {
		return useSsl_;
	}

	/** Get should use frame compression if available */
	bool CqlNodeConfiguration::getUseCompression() const {
		return useCompression_;
	}

	/** Get how many streams can hold in single connection */
	std::size_t CqlNodeConfiguration::getMaxStreams() const {
		return maxStreams_;
	}

	/** Get how many messages can hold in a received queue for single stream */
	std::size_t CqlNodeConfiguration::getMaxPendingMessages() const {
		return maxPendingMessages_;
	}

	/** Get the full authentication class name */
	const seastar::sstring& CqlNodeConfiguration::getAuthenticatorClass() const& {
		return authenticatorClass_;
	}

	/** Get the authentication data, the format depends on the class */
	const seastar::sstring& CqlNodeConfiguration::getAuthenticatorData() const& {
		return authenticatorData_;
	}
	
	/** Get the resolved ip address, return whether the ip address is available and not expired */
	bool CqlNodeConfiguration::getIpAddress(
		seastar::socket_address& ipAddress,
		std::chrono::milliseconds dnsCacheTime) const {
		if (ipAddressIsResolved_ &&
			(ipAddressIsFixed_ ||
			std::chrono::system_clock::now() - ipAddressUpdatedTime_ <= dnsCacheTime)) {
			ipAddress = ipAddress_;
			return true;
		}
		return false;
	}

	/** Update the resolved ip address */
	void CqlNodeConfiguration::updateIpAddress(const seastar::socket_address& ipAddress) {
		if (ipAddressIsFixed_) {
			throw CqlLogicException(CQL_CODEINFO, "should not update a fixed ip address");
		}
		ipAddress_ = ipAddress;
		ipAddressIsResolved_ = true;
		ipAddressUpdatedTime_ = std::chrono::system_clock::now();
	}

	/** Constructor */
	CqlNodeConfiguration::CqlNodeConfiguration() :
		address_(),
		useSsl_(false),
		useCompression_(false),
		maxStreams_(20),
		maxPendingMessages_(20),
		authenticatorClass_(CqlAuthenticatorClasses::AllowAllAuthenticator),
		authenticatorData_(),
		ipAddress_(),
		ipAddressIsResolved_(false),
		ipAddressIsFixed_(false),
		ipAddressUpdatedTime_() { }
}

