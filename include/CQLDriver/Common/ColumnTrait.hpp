#pragma once
#include <cstdint>
#include <cstring>
#include <type_traits>
#include <core/byteorder.hh>
#include "./ColumnTypes/Nullable.hpp"
#include "./Exceptions/DecodeException.hpp"

namespace cql {
	/** Trait class for cql column types */
	template <class T>
	class ColumnTrait {
	public:
		using UnderlyingType = typename T::UnderlyingType;
		using IsNullable = std::false_type;

		/** Hash function */
		struct Hash : private std::hash<UnderlyingType> {
			std::size_t operator()(const T& value) const {
				return std::hash<UnderlyingType>::operator()(value.get());
			}
		};

		/** Encode to binary data with size information */
		static void encode(const T& value, std::string& data) {
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
				throw DecodeException(CQL_CODEINFO,
					"length of column body size not enough, type:", typeid(T).name());
			}
			std::memcpy(&bodySize, ptr, sizeof(bodySize));
			bodySize = seastar::be_to_cpu(bodySize);
			if (bodySize >= 0) {
				if (end < ptr || end - ptr < sizeof(bodySize) + bodySize) {
					throw DecodeException(CQL_CODEINFO,
						"length of column body not enough, type:", typeid(T).name());
				}
				value.decodeBody(ptr + sizeof(bodySize), bodySize);
				ptr += sizeof(bodySize) + bodySize;
			} else {
				throw DecodeException(CQL_CODEINFO,
					"column is not nullable but the value is null, type:", typeid(T).name());
			}
		}
	};

	/** Trait class for nullable cql column types */
	template <class T>
	class ColumnTrait<Nullable<T>> {
	public:
		using UnderlyingType = typename T::UnderlyingType;
		using IsNullable = std::true_type;

		/** Encode to binary data with size information */
		static void encode(const Nullable<T>& value, std::string& data) {
			static const ColumnEncodeDecodeSizeType nullSize(
				seastar::cpu_to_be(static_cast<ColumnEncodeDecodeSizeType>(-1)));
			if (value.isNull()) {
				data.append(reinterpret_cast<const char*>(&nullSize), sizeof(nullSize));
			} else {
				ColumnTrait<T>::encode(value.get(), data);
			}
		}

		/** Decode from binary data with size information */
		static void decode(Nullable<T>& value, const char*& ptr, const char* end) {
			ColumnEncodeDecodeSizeType bodySize;
			if (ptr + sizeof(bodySize) > end) {
				throw DecodeException(CQL_CODEINFO,
					"length of column body size not enough, type:", typeid(T).name());
			}
			std::memcpy(&bodySize, ptr, sizeof(bodySize));
			bodySize = seastar::be_to_cpu(bodySize);
			if (bodySize >= 0) {
				if (end < ptr || end - ptr < sizeof(bodySize) + bodySize) {
					throw DecodeException(CQL_CODEINFO,
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

