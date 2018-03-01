#pragma once
#include <cstdint>
#include <type_traits>
#include <core/byteorder.hh>
#include "ColumnTypes/CqlNullable.hpp"
#include "Exceptions/CqlDecodeException.hpp"

namespace cql {
	/** Trait class for cql column types */
	template <class T>
	class CqlColumnTrait {
	public:
		using CqlUnderlyingType = typename T::CqlUnderlyingType;
		using CqlIsNullable = std::false_type;

		/** Encode to binary data with size information */
		static void encode(const T& value, seastar::sstring& data) {
			ColumnEncodeDecodeSizeType bodySize;
			std::size_t prevSize = data.size() + sizeof(bodySize);
			data.resize(prevSize);
			value.encodeBody(data);
			bodySize = seastar::cpu_to_be(
				static_cast<decltype(bodySize)>(data.size() - prevSize));
			std::memcpy(
				data.data() + prevSize - sizeof(bodySize),
				&bodySize, sizeof(bodySize));
		}

		/** Decode from binary data with size information */
		static void decode(T& value, const char*& ptr, const char* end) {
			ColumnEncodeDecodeSizeType bodySize;
			if (ptr + sizeof(bodySize) > end) {
				throw CqlDecodeException(CQL_CODEINFO,
					"length of column body size not enough, type:", typeid(T).name());
			}
			std::memcpy(&bodySize, ptr, sizeof(bodySize));
			bodySize = seastar::be_to_cpu(bodySize);
			if (bodySize >= 0) {
				if (ptr + sizeof(bodySize) + bodySize > end) {
					throw CqlDecodeException(CQL_CODEINFO,
						"length of column body not enough, type:", typeid(T).name());
				}
				value.decodeBody(ptr + sizeof(bodySize), bodySize);
				ptr += sizeof(bodySize) + bodySize;
			} else {
				throw CqlDecodeException(CQL_CODEINFO,
					"column is not nullable but the value is null, type:", typeid(T).name());
			}
		}
	};

	/** Trait class for nullable cql column types */
	template <class T>
	class CqlColumnTrait<CqlNullable<T>> {
	public:
		using CqlUnderlyingType = typename T::CqlUnderlyingType;
		using CqlIsNullable = std::true_type;

		/** Encode to binary data with size information */
		static void encode(const CqlNullable<T>& value, seastar::sstring& data) {
			static const ColumnEncodeDecodeSizeType nullSize(
				seastar::cpu_to_be(static_cast<ColumnEncodeDecodeSizeType>(-1)));
			if (value.isNull()) {
				data.append(reinterpret_cast<const char*>(&nullSize), sizeof(nullSize));
			} else {
				CqlColumnTrait<T>::encode(value.get(), data);
			}
		}

		/** Decode from binary data with size information */
		static void decode(CqlNullable<T>& value, const char*& ptr, const char* end) {
			ColumnEncodeDecodeSizeType bodySize;
			if (ptr + sizeof(bodySize) > end) {
				throw CqlDecodeException(CQL_CODEINFO,
					"length of column body size not enough, type:", typeid(T).name());
			}
			std::memcpy(&bodySize, ptr, sizeof(bodySize));
			bodySize = seastar::be_to_cpu(bodySize);
			if (bodySize >= 0) {
				if (ptr + sizeof(bodySize) + bodySize > end) {
					throw CqlDecodeException(CQL_CODEINFO,
						"length of column body not enough, type:", typeid(T).name());
				}
				value.decodeBody(ptr + sizeof(bodySize), bodySize);
				ptr += sizeof(bodySize) + bodySize;
			} else {
				value.reset();
				ptr += sizeof(bodySize);
			}
		}
	};
}

