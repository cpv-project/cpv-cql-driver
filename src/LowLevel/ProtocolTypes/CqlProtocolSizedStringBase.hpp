#pragma once
#include <cstdint>
#include <limits>
#include <core/sstring.hh>
#include <core/byteorder.hh>
#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include <CqlDriver/Common/Exceptions/CqlEncodeException.hpp>

namespace cql {
	/**
	 * Base class of sized string types
	 */
	template <class LengthType, class StateType, StateType DefaultState, StateType NormalState>
	class CqlProtocolSizedStringBase {
	public:
		/** Get the value of string, only available when state is NormalState */
		const seastar::sstring& get() const& { return value_; }

		/** Set the value of string */
		seastar::sstring& get() & { return value_; }

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
		void set(const seastar::sstring& value) {
			state_ = NormalState;
			value_ = value;
		}

		/** Set the value of string and set the state to NormalState */
		void set(seastar::sstring&& value) {
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
		void encode(seastar::sstring& data) const {
			if (state_ != NormalState) {
				LengthType size = seastar::cpu_to_be(static_cast<LengthType>(state_));
				data.append(reinterpret_cast<const char*>(&size), sizeof(size));
			} else {
				if (value_.size() > static_cast<std::size_t>(std::numeric_limits<LengthType>::max())) {
					throw CqlEncodeException(CQL_CODEINFO, "length too long");
				}
				LengthType size = seastar::cpu_to_be(static_cast<LengthType>(value_.size()));
				data.append(reinterpret_cast<const char*>(&size), sizeof(size));
				data.append(value_.c_str(), value_.size());
			}
		}

		/** Decode from binary data */
		void decode(const char*& ptr, const char* end) {
			LengthType size = 0;
			if (ptr + sizeof(size) > end) {
				throw CqlDecodeException(CQL_CODEINFO, "length not enough");
			}
			std::memcpy(&size, ptr, sizeof(size));
			size = seastar::be_to_cpu(size);
			ptr += sizeof(size);
			if (std::is_unsigned<LengthType>::value || size >= 0) {
				if (ptr + static_cast<std::size_t>(size) > end) {
					throw CqlDecodeException(CQL_CODEINFO, "length not enough");
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
		CqlProtocolSizedStringBase() : value_(), state_(DefaultState) { }
		explicit CqlProtocolSizedStringBase(StateType state) : value_(), state_(state) { }
		explicit CqlProtocolSizedStringBase(seastar::sstring&& value) :
			value_(std::move(value)), state_(NormalState) {}

	protected:
		seastar::sstring value_;
		StateType state_;
	};
}

