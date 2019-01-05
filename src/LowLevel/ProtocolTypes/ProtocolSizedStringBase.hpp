#pragma once
#include <cstdint>
#include <cstring>
#include <limits>
#include <string>
#include <seastar/core/byteorder.hh>
#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <CQLDriver/Common/Exceptions/EncodeException.hpp>
#include <CQLDriver/Common/CommonDefinitions.hpp>

namespace cql {
	/** Base class of sized string types */
	template <class LengthType, class StateType, StateType DefaultState, StateType NormalState>
	class ProtocolSizedStringBase {
	public:
		/** Get the value of string, only available when state is NormalState */
		const std::string& get() const& { return value_; }

		/** Get the mutable value of string */
		std::string& get() & { return value_; }

		/** Get the state */
		StateType state() const { return state_; }

		/** Set the state and clear the value of string */
		void set(StateType state) {
			state_ = state;
			value_.resize(0);
		}

		/** Set the value of string and set the state to NormalState */
		void set(const char* str, std::size_t size) {
			state_ = NormalState;
			value_.resize(0);
			value_.append(str, size);
		}

		/** Set the value of string and set the state to NormalState */
		void set(const std::string& value) {
			state_ = NormalState;
			value_ = value;
		}

		/** Set the value of string and set the state to NormalState */
		void set(std::string&& value) {
			state_ = NormalState;
			value_ = std::move(value);
		}

		/** Append the value of string and set the state to NormalState */
		void append(const char* str, std::size_t size) {
			state_ = NormalState;
			value_.append(str, size);
		}

		/** Reset to initial state */
		void reset() {
			state_ = DefaultState;
			value_.resize(0);
		}

		/** Encode to binary data */
		void encode(std::string& data) const {
			if (state_ != NormalState) {
				LengthType size = seastar::cpu_to_be(static_cast<LengthType>(state_));
				data.append(reinterpret_cast<const char*>(&size), sizeof(size));
			} else {
				if (CQL_UNLIKELY(value_.size() >
					static_cast<std::size_t>(std::numeric_limits<LengthType>::max()))) {
					throw EncodeException(CQL_CODEINFO, "length too long");
				}
				LengthType size = seastar::cpu_to_be(static_cast<LengthType>(value_.size()));
				data.append(reinterpret_cast<const char*>(&size), sizeof(size));
				data.append(value_.c_str(), value_.size());
			}
		}

		/** Decode from binary data */
		void decode(const char*& ptr, const char* end) {
			LengthType size = 0;
			if (CQL_UNLIKELY(ptr + sizeof(size) > end)) {
				throw DecodeException(CQL_CODEINFO, "length not enough");
			}
			std::memcpy(&size, ptr, sizeof(size));
			size = seastar::be_to_cpu(size);
			ptr += sizeof(size);
			// cppcheck-suppress unsignedPositive
			if (std::is_unsigned<LengthType>::value || size >= 0) {
				if (CQL_UNLIKELY(end < ptr || end - ptr < static_cast<std::ptrdiff_t>(size))) {
					throw DecodeException(CQL_CODEINFO, "length not enough");
				}
				state_ = NormalState;
				value_.resize(0);
				value_.append(ptr, static_cast<std::size_t>(size));
				ptr += static_cast<std::size_t>(size);
			} else {
				state_ = static_cast<StateType>(size);
				value_.resize(0);
			}
		}

		/** Constructors */
		ProtocolSizedStringBase() : value_(), state_(DefaultState) { }
		explicit ProtocolSizedStringBase(StateType state) : value_(), state_(state) { }
		explicit ProtocolSizedStringBase(std::string&& value) :
			value_(std::move(value)), state_(NormalState) {}

	protected:
		std::string value_;
		StateType state_;
	};
}

