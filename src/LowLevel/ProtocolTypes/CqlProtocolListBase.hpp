#pragma once
#include <cstdint>
#include <vector>
#include <limits>
#include <core/sstring.hh>
#include <core/byteorder.hh>
#include <CqlDriver/Common/Exceptions/CqlEncodeException.hpp>

namespace cql {
	/**
	 * Base class of list types
	 */
	template <class LengthType, class ElementType>
	class CqlProtocolListBase {
	public:
		static const std::size_t SmallSizeBoundary = 255;
		using VectorType = std::vector<ElementType>;

		const VectorType& get() const& { return value_; }
		VectorType& get() & { return value_; }
		void reset() { value_.clear(); }

		void encode(seastar::sstring& data) const {
			if (value_.size() > static_cast<std::size_t>(std::numeric_limits<LengthType>::max())) {
				throw CqlEncodeException(CQL_CODEINFO, "length too long");
			}
			LengthType size = seastar::cpu_to_be(static_cast<LengthType>(value_.size()));
			data.append(reinterpret_cast<const char*>(&size), sizeof(size));
			for (const auto& element : value_) {
				element.encode(data);
			}
		}

		void decode(const char*& ptr, const char* end) {
			LengthType size = 0;
			if (ptr + sizeof(size) > end) {
				throw CqlDecodeException(CQL_CODEINFO, "length not enough");
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

		CqlProtocolListBase() : value_() { }
		explicit CqlProtocolListBase(VectorType&& value) : value_(std::move(value)) { }

	protected:
		VectorType value_;
	};
}

