#pragma once
#include <cstdint>
#include <vector>
#include <core/sstring.hh>
#include "../Exceptions/CqlDecodeException.hpp"

namespace cql {
	template <class T>
	class CqlList {
	public:
		using CqlUnderlyingType = std::vector<T>;

		/** Get the list value */
		const CqlUnderlyingType& get() const& { return value_; }

		/** Get the mutable list value */
		CqlUnderlyingType& get() & { return value_; }

		/** Set the list value */
		void set(const CqlUnderlyingType& value) { value_ = value_; }

		/** Set the list value */
		void set(CqlUnderlyingType&& value) { value_ = std::move(value); }

		/** Reset to initial state */
		void reset() { value_.resize(0); }

		/** Encode to binary data */
		void encode(seastar::sstring& data) const {
			


		}

		/** Decode from binary data */
		void decode(const char* ptr, std::int32_t size) {
			
		}

	private:
		CqlUnderlyingType value_;
	};
}

