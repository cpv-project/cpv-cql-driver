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
		seastar::net::inet_address get() const { return value_; }
		void set(const seastar::net::inet_address& value) & { value_ = value; }

		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		CqlProtocolInetAddr() : value_() { }
		explicit CqlProtocolInetAddr(const seastar::net::inet_address& value) :
			value_(value) { }

	protected:
		seastar::net::inet_address value_;
	};
}

