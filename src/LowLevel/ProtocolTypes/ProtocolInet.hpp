#pragma once
#include <utility>
#include "./ProtocolInetAddr.hpp"
#include "./ProtocolInt.hpp"

namespace cql {
	/**
	 * An IP address (ip and port) to a node
	 * It consists of one [byte] n, that represents the address size,
	 * followed by n [byte] representing the IP address,
	 * followed by one [int] representing the port.
	 */
	class ProtocolInet {
	public:
		using ValueType = std::pair<seastar::net::inet_address, std::uint16_t>;

		/** Get the ip address and port */
		ValueType get() const {
			return std::make_pair(address_.get(), static_cast<std::uint16_t>(port_.get()));
		}

		/** Set the ip address and port */
		void set(const ValueType& value) {
			address_ = ProtocolInetAddr(value.first);
			port_ = ProtocolInt(value.second);
		}

		/** Reset to initial state */
		void reset() {
			address_.reset();
			port_.reset();
		}

		/** Encode to binary data */
		void encode(seastar::sstring& data) const {
			address_.encode(data);
			port_.encode(data);
		}

		/** Decode from binary data */
		void decode(const char*& ptr, const char* end) {
			address_.decode(ptr, end);
			port_.decode(ptr, end);
		}

		/** Constructors */
		ProtocolInet() : address_(), port_(0) { }
		explicit ProtocolInet(const ValueType& value) :
			address_(value.first), port_(value.second) { }

	private:
		ProtocolInetAddr address_;
		ProtocolInt port_;
	};
}

