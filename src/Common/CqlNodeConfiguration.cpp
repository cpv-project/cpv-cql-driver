#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include <CqlDriver/Common/Exceptions/CqlNotImplementedException.hpp>
#include <CqlDriver/Common/Exceptions/CqlFormatException.hpp>
#include <CqlDriver/Common/CqlCommonDefinitions.hpp>
#include <CqlDriver/Common/CqlNodeConfiguration.hpp>
#include <stdexcept>
#include <net/inet_address.hh>

namespace cql {
	/** Defines members of CqlNodeConfiguration */
	class CqlNodeConfigurationData {
	public:
		CqlNodeConfigurationData() :
			address(),
			useSsl(false),
			useCompression(false),
			maxStreams(20),
			maxPendingMessages(20),
			authenticatorClass(CqlAuthenticatorClasses::AllowAllAuthenticator),
			authenticatorData(),
			ipAddress(),
			ipAddressIsResolved(false),
			ipAddressIsFixed(false),
			ipAddressUpdatedTime() { }

		std::pair<seastar::sstring, std::uint16_t> address;
		bool useSsl;
		bool useCompression;
		std::size_t maxStreams;
		std::size_t maxPendingMessages;
		seastar::sstring authenticatorClass;
		seastar::sstring authenticatorData;
		seastar::socket_address ipAddress;
		bool ipAddressIsResolved;
		bool ipAddressIsFixed;
		std::chrono::system_clock::time_point ipAddressUpdatedTime;
	};

	/** Set the hostname and the port of this node */
	CqlNodeConfiguration& CqlNodeConfiguration::setAddress(
		seastar::sstring&& hostname, std::uint16_t port) {
		data_->address = std::make_pair(hostname, port);
		try {
			seastar::net::inet_address inetAddress(hostname);
			if (inetAddress.in_family() == seastar::net::inet_address::family::INET) {
				data_->ipAddress = seastar::socket_address(seastar::ipv4_addr(inetAddress, port));
				data_->ipAddressIsResolved = true;
				data_->ipAddressIsFixed = true;
			} else {
				// seastar's socket_address only support ipv4 now so throw a exception for ipv6
				throw CqlNotImplementedException(CQL_CODEINFO, "ipv6 address isn't supported yet");
			}
		} catch (const std::invalid_argument&) {
			data_->ipAddressIsResolved = false;
			data_->ipAddressIsFixed = false;
			data_->ipAddressUpdatedTime = {};
		}
		return *this;
	}

	/** Set should connect this node with ssl connection */
	CqlNodeConfiguration& CqlNodeConfiguration::setUseSsl(bool value) {
		data_->useSsl = value;
		return *this;
	}

	/** Set should use frame compression if available */
	CqlNodeConfiguration& CqlNodeConfiguration::setUseCompression(bool value) {
		data_->useCompression = value;
		return *this;
	}

	/** Set how many streams can hold in single connection */
	CqlNodeConfiguration& CqlNodeConfiguration::setMaxStreams(std::size_t value) {
		if (!(value >= 2 && value <= 255)) {
			throw CqlFormatException(CQL_CODEINFO,
				"invalid max streams value, it should >= 2 and <= 255");
		}
		data_->maxStreams = value;
		return *this;
	}

	/** Set how many messages can hold in a received queue for single stream */
	CqlNodeConfiguration& CqlNodeConfiguration::setMaxPendingMessages(std::size_t value) {
		if (!(value >= 1)) {
			throw CqlFormatException(CQL_CODEINFO,
				"invalid max pending messages value, it should >= 1");
		}
		data_->maxPendingMessages = value;
		return *this;
	}

	/** Set to use password authentication for this node */
	CqlNodeConfiguration& CqlNodeConfiguration::setPasswordAuthentication(
		seastar::sstring&& username, seastar::sstring&& password) {
		data_->authenticatorClass = CqlAuthenticatorClasses::PasswordAuthenticator;
		data_->authenticatorData.resize(0);
		data_->authenticatorData.append("\x00", 1);
		data_->authenticatorData.append(username.c_str(), username.size());
		data_->authenticatorData.append("\x00", 1);
		data_->authenticatorData.append(password.c_str(), password.size());
		return *this;
	}

	/** Get the hostname and the port of this node */
	const std::pair<seastar::sstring, std::uint16_t>& CqlNodeConfiguration::getAddress() const& {
		return data_->address;
	}

	/** Get should connect this node with ssl connection */
	bool CqlNodeConfiguration::getUseSsl() const {
		return data_->useSsl;
	}

	/** Get should use frame compression if available */
	bool CqlNodeConfiguration::getUseCompression() const {
		return data_->useCompression;
	}

	/** Get how many streams can hold in single connection */
	std::size_t CqlNodeConfiguration::getMaxStreams() const {
		return data_->maxStreams;
	}

	/** Get how many messages can hold in a received queue for single stream */
	std::size_t CqlNodeConfiguration::getMaxPendingMessages() const {
		return data_->maxPendingMessages;
	}

	/** Get the full authentication class name */
	const seastar::sstring& CqlNodeConfiguration::getAuthenticatorClass() const& {
		return data_->authenticatorClass;
	}

	/** Get the authentication data, the format depends on the class */
	const seastar::sstring& CqlNodeConfiguration::getAuthenticatorData() const& {
		return data_->authenticatorData;
	}
	
	/** Get the resolved ip address, return whether the ip address is available and not expired */
	bool CqlNodeConfiguration::getIpAddress(
		seastar::socket_address& ipAddress,
		std::chrono::milliseconds dnsCacheTime) const {
		if (data_->ipAddressIsResolved &&
			(data_->ipAddressIsFixed ||
			std::chrono::system_clock::now() - data_->ipAddressUpdatedTime <= dnsCacheTime)) {
			ipAddress = data_->ipAddress;
			return true;
		}
		return false;
	}

	/** Update the resolved ip address */
	void CqlNodeConfiguration::updateIpAddress(const seastar::socket_address& ipAddress) {
		if (data_->ipAddressIsFixed) {
			throw CqlLogicException(CQL_CODEINFO, "should not update a fixed ip address");
		}
		data_->ipAddress = ipAddress;
		data_->ipAddressIsResolved = true;
		data_->ipAddressUpdatedTime = std::chrono::system_clock::now();
	}

	/** Constructor */
	CqlNodeConfiguration::CqlNodeConfiguration() :
		data_(std::make_shared<CqlNodeConfigurationData>()) { }
}

