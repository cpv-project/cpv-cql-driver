#include <CQLDriver/Common/NodeConfiguration.hpp>
#include <CQLDriver/Common/Exceptions/LogicException.hpp>
#include <CQLDriver/Common/Exceptions/NotImplementedException.hpp>
#include <CQLDriver/Common/Exceptions/FormatException.hpp>
#include <CQLDriver/Common/Utility/StringUtils.hpp>
#include <CQLDriver/Common/CommonDefinitions.hpp>
#include <stdexcept>
#include <seastar/net/inet_address.hh>

namespace cql {
	/** Defines members of NodeConfiguration */
	class NodeConfigurationData {
	public:
		NodeConfigurationData() :
			address(),
			useSSL(false),
			useCompression(false),
			maxStreams(20),
			maxPendingMessages(100),
			authenticatorClass(AuthenticatorClasses::AllowAllAuthenticator),
			authenticatorData(),
			keepaliveParameters(),
			ipAddress(),
			ipAddressIsResolved(false),
			ipAddressIsFixed(false),
			ipAddressUpdatedTime(),
			preparedQueryIds() { }

		std::pair<std::string, std::uint16_t> address;
		bool useSSL;
		bool useCompression;
		std::size_t maxStreams;
		std::size_t maxPendingMessages;
		std::string authenticatorClass;
		std::string authenticatorData;
		std::optional<seastar::net::keepalive_params> keepaliveParameters;
		seastar::socket_address ipAddress;
		bool ipAddressIsResolved;
		bool ipAddressIsFixed;
		std::chrono::system_clock::time_point ipAddressUpdatedTime;
		std::unordered_map<StringHolder, std::string, StringHolder::Hash> preparedQueryIds;
	};

	/** Set the hostname and the port of this node */
	NodeConfiguration& NodeConfiguration::setAddress(
		std::string&& hostname, std::uint16_t port) {
		data_->address = std::make_pair(hostname, port);
		try {
			seastar::net::inet_address inetAddress(hostname);
			if (CQL_LIKELY(inetAddress.in_family() == seastar::net::inet_address::family::INET)) {
				data_->ipAddress = seastar::socket_address(seastar::ipv4_addr(inetAddress, port));
				data_->ipAddressIsResolved = true;
				data_->ipAddressIsFixed = true;
			} else {
				// seastar's socket_address only support ipv4 now so throw a exception for ipv6
				throw NotImplementedException(CQL_CODEINFO, "ipv6 address isn't supported yet");
			}
		} catch (const std::invalid_argument&) {
			data_->ipAddressIsResolved = false;
			data_->ipAddressIsFixed = false;
			data_->ipAddressUpdatedTime = {};
		}
		return *this;
	}

	/** Set should connect this node with ssl connection */
	NodeConfiguration& NodeConfiguration::setUseSSL(bool value) {
		data_->useSSL = value;
		return *this;
	}

	/** Set should use frame compression if available */
	NodeConfiguration& NodeConfiguration::setUseCompression(bool value) {
		data_->useCompression = value;
		return *this;
	}

	/** Set how many streams can hold in a single connection */
	NodeConfiguration& NodeConfiguration::setMaxStreams(std::size_t value) {
		if (CQL_UNLIKELY(!(value >= 2 && value <= 255))) {
			throw FormatException(CQL_CODEINFO,
				"invalid max streams value, it should >= 2 and <= 255");
		}
		data_->maxStreams = value;
		return *this;
	}

	/** Set how many messages can hold in a received queue for a single stream */
	NodeConfiguration& NodeConfiguration::setMaxPendingMessages(std::size_t value) {
		if (CQL_UNLIKELY(!(value >= 1))) {
			throw FormatException(CQL_CODEINFO,
				"invalid max pending messages value, it should >= 1");
		}
		data_->maxPendingMessages = value;
		return *this;
	}

	/** Set to use password authentication for this node */
	NodeConfiguration& NodeConfiguration::setPasswordAuthentication(
		std::string&& username, std::string&& password) {
		data_->authenticatorClass = AuthenticatorClasses::PasswordAuthenticator;
		data_->authenticatorData.resize(0);
		data_->authenticatorData.append("\x00", 1);
		data_->authenticatorData.append(username.c_str(), username.size());
		data_->authenticatorData.append("\x00", 1);
		data_->authenticatorData.append(password.c_str(), password.size());
		return *this;
	}

	/** Set keepalive parameters of connection */
	NodeConfiguration& NodeConfiguration::setKeepaliveParameters(
		std::optional<seastar::net::keepalive_params>&& keepaliveParameters) {
		data_->keepaliveParameters = std::move(keepaliveParameters);
		return *this;
	}

	/** Get the hostname and the port of this node */
	const std::pair<std::string, std::uint16_t>& NodeConfiguration::getAddress() const& {
		return data_->address;
	}

	/** Get the string representation of the hostname and the port of this node */
	std::string NodeConfiguration::getAddressAsString() const {
		return joinString("", data_->address.first, ':', data_->address.second);
	}

	/** Get should connect this node with ssl connection */
	bool NodeConfiguration::getUseSSL() const {
		return data_->useSSL;
	}

	/** Get should use frame compression if available */
	bool NodeConfiguration::getUseCompression() const {
		return data_->useCompression;
	}

	/** Get how many streams can hold in a single connection */
	std::size_t NodeConfiguration::getMaxStreams() const {
		return data_->maxStreams;
	}

	/** Get how many messages can hold in a received queue for a single stream */
	std::size_t NodeConfiguration::getMaxPendingMessages() const {
		return data_->maxPendingMessages;
	}

	/** Get the full authentication class name */
	const std::string& NodeConfiguration::getAuthenticatorClass() const& {
		return data_->authenticatorClass;
	}

	/** Get the authentication data, the format depends on the class */
	const std::string& NodeConfiguration::getAuthenticatorData() const& {
		return data_->authenticatorData;
	}

	/** Get keepalive parameters of connection */
	const std::optional<seastar::net::keepalive_params>& NodeConfiguration::getKeepaliveParameters() const& {
		return data_->keepaliveParameters;
	}

	/** Get the resolved ip address, return whether the ip address is available and not expired */
	bool NodeConfiguration::getIpAddress(
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
	void NodeConfiguration::updateIpAddress(const seastar::socket_address& ipAddress) {
		if (CQL_UNLIKELY(data_->ipAddressIsFixed)) {
			throw LogicException(CQL_CODEINFO, "should not update a fixed ip address");
		}
		data_->ipAddress = ipAddress;
		data_->ipAddressIsResolved = true;
		data_->ipAddressUpdatedTime = std::chrono::system_clock::now();
	}

	/** Get the prepared query id cached in this node for the specificed query string */
	std::string& NodeConfiguration::getPreparedQueryId(const StringHolder& queryStr) & {
		return data_->preparedQueryIds[queryStr];
	}

	/** Constructor */
	NodeConfiguration::NodeConfiguration() :
		data_(std::make_unique<NodeConfigurationData>()) { }

	/** Destructor */
	NodeConfiguration::~NodeConfiguration() = default;

	/** Copy constructor */
	NodeConfiguration::NodeConfiguration(const NodeConfiguration& other) :
		data_(std::make_unique<NodeConfigurationData>(*other.data_)) { }

	/** Move constructor */
	NodeConfiguration::NodeConfiguration(NodeConfiguration&& other) :
		NodeConfiguration(other) {
		// Same as copy, disallow empty pointer
	}

	/** Copy assignment */
	NodeConfiguration& NodeConfiguration::operator=(const NodeConfiguration& other) {
		if (this != &other) {
			data_ = std::make_unique<NodeConfigurationData>(*other.data_);
		}
		return *this;
	}

	/** Move assignment */
	NodeConfiguration& NodeConfiguration::operator=(NodeConfiguration&& other) {
		// Same as copy, disallow empty pointer
		return *this = other;
	}
}

