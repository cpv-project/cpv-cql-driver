#pragma once
#include <cstdint>
#include <core/sstring.hh>
#include <net/inet_address.hh>
#include "../Exceptions/DecodeException.hpp"
#include "./ColumnDefinitions.hpp"

namespace cql {
	/** A 4 byte or 16 byte sequence denoting an IPv4 or IPv6 address */
	class Inet {
	public:
		using UnderlyingType = seastar::net::inet_address;

		/** Get the ip address */
		const UnderlyingType& get() const& { return value_; }

		/** Get the mutable ip address */
		UnderlyingType& get() & { return value_; }

		/** Set the ip address */
		void set(const UnderlyingType& value) { value_ = value; }

		/** Set the ip address by it's string representation */
		void set(const seastar::sstring& value) { value_ = UnderlyingType(value); }

		/** Reset to initial state */
		void reset() { value_ = {}; }

		/** Encode to binary data */
		void encodeBody(seastar::sstring& data) const {
			data.append(reinterpret_cast<const char*>(value_.data()), value_.size());
		}

		/** Decode from binary data */
		void decodeBody(const char* ptr, ColumnEncodeDecodeSizeType size) {
			if (size == 0) {
				reset(); // empty
			} else if (size == sizeof(::in_addr)) {
				::in_addr addr_v4;
				std::memcpy(&addr_v4, ptr, size);
				value_ = UnderlyingType(addr_v4);
			} else if (size == sizeof(::in6_addr)) {
				::in6_addr addr_v6;
				std::memcpy(&addr_v6, ptr, size);
				value_ = UnderlyingType(addr_v6);
			} else {
				throw DecodeException(CQL_CODEINFO,
					"unsupported inet address length", size);
			}
		}

		/** Constructors */
		Inet() : value_() { }
		template <class... Args>
		explicit Inet(Args&&... args) : value_() {
			set(std::forward<Args>(args)...);
		}

		/** Allow assign from whatever function "set" would take */
		template <class... Args>
		void operator=(Args&&... args) {
			set(std::forward<Args>(args)...);
		}

		/** Allow cast to ip address implicitly */
		operator UnderlyingType() const {
			return value_;
		}

		/** Dereference operation */
		const UnderlyingType& operator*() const& { return value_; }
		UnderlyingType& operator*() & { return value_; }

		/** Get pointer operation */
		const UnderlyingType* operator->() const& { return &value_; }
		UnderlyingType* operator->() & { return &value_; }

	private:
		UnderlyingType value_;
	};

	/** Compare operations of ip address */
	static bool operator==(const Inet& a, const Inet& b) { return a.get() == b.get(); }
	static bool operator==(const Inet& a, const Inet::UnderlyingType& b) { return a.get() == b; }
	static bool operator==(const Inet::UnderlyingType& a, const Inet& b) { return a == b.get(); }
	static bool operator!=(const Inet& a, const Inet& b) { return !(a.get() == b.get()); }
	static bool operator!=(const Inet& a, const Inet::UnderlyingType& b) { return !(a.get() == b); }
	static bool operator!=(const Inet::UnderlyingType& a, const Inet& b) { return !(a == b.get()); }

	/** Write text description of ip address */
	static std::ostream& operator<<(std::ostream& stream, const Inet& value) {
		return stream << value.get();
	}
}

