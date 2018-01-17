#pragma once
#include <cstdint>
#include <net/inet_address.hh>

namespace cql {
	/**
	 * An IP address (without a port) to a node
	 * it consists of one [byte] n, that represents the address size,
	 * followed by n [byte] representing the IP address.
	 */
	class CqlProtocolInetAddr {
	public:
		/** Get the ip address */
		seastar::net::inet_address get() const { return value_; }

		/** Set the ip address */
		void set(const seastar::net::inet_address& value) & { value_ = value; }

		/** Reset to initial state */
		void reset() { value_ = {}; }

		/** Encode and decode functions */
		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		/** Constructors */
		CqlProtocolInetAddr() : value_() { }
		explicit CqlProtocolInetAddr(const seastar::net::inet_address& value) :
			value_(value) { }

	private:
		seastar::net::inet_address value_;
	};
}

