#pragma once
#include <utility>
#include "CqlProtocolInetAddr.hpp"
#include "CqlProtocolInt.hpp"

namespace cql {
	class CqlProtocolInet {
	public:
		using ValueType = std::pair<seastar::net::inet_address, std::uint16_t>;

		ValueType get() const {
			return std::make_pair(address_.get(), static_cast<std::uint16_t>(port_.get()));
		}

		void set(const ValueType& value) {
			address_ = CqlProtocolInetAddr(value.first);
			port_ = CqlProtocolInt(value.second);
		}

		void encode(seastar::sstring& data) const {
			address_.encode(data);
			port_.encode(data);
		}

		void decode(const char*& ptr, const char* end) {
			address_.decode(ptr, end);
			port_.decode(ptr, end);
		}

		CqlProtocolInet() : address_(), port_(0) { }
		CqlProtocolInet(const ValueType& value) :
			address_(value.first), port_(value.second) { }

	private:
		CqlProtocolInetAddr address_;
		CqlProtocolInt port_;
	};
}

