#pragma once
#include <cstdint>
#include <cstring>
#include <vector>
#include <limits>
#include <core/byteorder.hh>
#include <CQLDriver/Common/Exceptions/EncodeException.hpp>
#include <CQLDriver/Common/Exceptions/DecodeException.hpp>

namespace cql {
	/**
	 * Base class of list types
	 */
	template <class LengthType, class ElementType>
	class ProtocolListBase {
	public:
		static const std::size_t SmallSizeBoundary = 255;
		using VectorType = std::vector<ElementType>;

		/** Get the values of list */
		const VectorType& get() const& { return value_; }
		VectorType& get() & { return value_; }

		/** Set the values of list */
		void set(const VectorType& value) { value_ = value; }
		void set(VectorType&& value) { value_ = std::move(value); }

		/** Reset to initial state */
		void reset() { value_.clear(); }

		/** Encode to binary data */
		void encode(std::string& data) const {
			if (value_.size() > static_cast<std::size_t>(std::numeric_limits<LengthType>::max())) {
				throw EncodeException(CQL_CODEINFO, "length too long");
			}
			LengthType size = seastar::cpu_to_be(static_cast<LengthType>(value_.size()));
			data.append(reinterpret_cast<const char*>(&size), sizeof(size));
			for (const auto& element : value_) {
				element.encode(data);
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
			if (static_cast<std::size_t>(size) <= SmallSizeBoundary) {
				// size is small, do the optimization
				value_.resize(static_cast<std::size_t>(size));
				for (auto& element : value_) {
					element.decode(ptr, end);
				}
			} else {
				// size is big, append each for security concern
				value_.resize(SmallSizeBoundary);
				for (std::size_t i = 0; i < static_cast<std::size_t>(size); ++i) {
					if (i < value_.size()) {
						value_[i].decode(ptr, end);
					} else {
						value_.emplace_back();
						value_.back().decode(ptr, end);
					}
				}
			}
		}

		/** Constructors */
		ProtocolListBase() : value_() { }
		explicit ProtocolListBase(VectorType&& value) :
			value_(std::move(value)) { }

	protected:
		VectorType value_;
	};
}

