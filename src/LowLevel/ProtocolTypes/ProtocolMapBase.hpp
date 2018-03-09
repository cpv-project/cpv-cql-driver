#pragma once
#include <unordered_map>
#include <core/sstring.hh>
#include <core/byteorder.hh>
#include <CQLDriver/Common/Exceptions/EncodeException.hpp>
#include <CQLDriver/Common/Exceptions/DecodeException.hpp>

namespace cql {
	/** Default hash function for map key */
	template <class KeyType>
	struct ProtocolMapBaseDefaultHash :
		private std::hash<std::decay_t<decltype(std::declval<KeyType>().get())>> {
		std::size_t operator()(const KeyType& key) const {
			return std::hash<std::decay_t<decltype(key.get())>>::operator()(key.get());
		}
	};

	/** Default compare function for map key */
	template <class KeyType>
	struct ProtocolMapBaseDefaultPred {
		bool operator()(const KeyType& lhs, const KeyType& rhs) const {
			return lhs.get() == rhs.get();
		}
	};

	/**
	 * Base class of map types
	 */
	template <class LengthType,
		class KeyType,
		class ValueType,
		class HashType = ProtocolMapBaseDefaultHash<KeyType>,
		class PredType = ProtocolMapBaseDefaultPred<KeyType>>
	class ProtocolMapBase {
	public:
		using MapType = std::unordered_map<KeyType, ValueType, HashType, PredType>;

		/** Get the keys and values of map */
		const MapType& get() const& { return value_; }
		MapType& get() & { return value_; }

		/** Set the keys and values of map */
		void set(const MapType& value) { value_ = value; }
		void set(MapType&& value) { value_ = std::move(value); }

		/** Reset to initial state */
		void reset() { value_.clear(); }

		/** Encode to binary data */
		void encode(seastar::sstring& data) const {
			if (value_.size() > static_cast<std::size_t>(std::numeric_limits<LengthType>::max())) {
				throw EncodeException(CQL_CODEINFO, "length too long");
			}
			LengthType size = seastar::cpu_to_be(static_cast<LengthType>(value_.size()));
			data.append(reinterpret_cast<const char*>(&size), sizeof(size));
			for (const auto& pair : value_) {
				pair.first.encode(data);
				pair.second.encode(data);
			}
		}

		/** Decode from binary data */
		void decode(const char*& ptr, const char* end) {
			LengthType size = 0;
			if (ptr + sizeof(size) > end) {
				throw DecodeException(CQL_CODEINFO, "length not enough");
			}
			std::memcpy(&size, ptr, sizeof(size));
			size = seastar::be_to_cpu(size);
			ptr += sizeof(size);
			value_.clear();
			KeyType key;
			ValueType value;
			for (std::size_t i = 0, j = static_cast<std::size_t>(size); i < j; ++i) {
				key.decode(ptr, end);
				value.decode(ptr, end);
				value_.emplace(std::move(key), std::move(value));
			}
		}

		/** Constructors */
		ProtocolMapBase() : value_() { }
		explicit ProtocolMapBase(MapType&& value) :
			value_(std::move(value)) { }

	protected:
		MapType value_;
	};
}

