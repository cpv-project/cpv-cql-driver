#include <array>
#include <core/byteorder.hh>
#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include "CqlProtocolVariableIntegerBase.hpp"

namespace cql {
	namespace {
		static const std::array<std::uint8_t, 9> ExtraBytesHeaders({
			0b00000000,
			0b10000000,
			0b11000000,
			0b11100000,
			0b11110000,
			0b11111000,
			0b11111100,
			0b11111110,
			0b11111111
		});
	}

	void CqlProtocolVariableIntegerBase::encode(seastar::sstring& data) const {
		std::uint64_t valueBe = seastar::cpu_to_be(value_);
		const char* ptr = reinterpret_cast<const char*>(&valueBe);
		const char* end = ptr + sizeof(valueBe);
		// trim empty bytes and calculate total bytes
		while (ptr < end && *ptr == 0) {
			++ptr;
		}
		std::size_t totalBytes = end - ptr;
		if (totalBytes == 0) {
			// value is 0
			data.append("\x00", 1);
		} else {
			// check the header can embed to the first byte
			std::uint8_t header = ExtraBytesHeaders[totalBytes];
			if ((header & static_cast<std::uint8_t>(*ptr)) != 0) {
				data.append(reinterpret_cast<const char*>(&header), 1);
				data.append(ptr, totalBytes);
			} else {
				header = *ptr | ExtraBytesHeaders[totalBytes-1];
				data.append(reinterpret_cast<const char*>(&header), 1);
				data.append(ptr+1, totalBytes-1);
			}
		}
	}

	void CqlProtocolVariableIntegerBase::decode(const char*& ptr, const char* end) {
		if (ptr + sizeof(char) > end) {
			throw CqlDecodeException(CQL_CODEINFO, "length not enough");
		}
		// read header
		std::uint8_t header = static_cast<std::uint8_t>(*ptr);
		std::size_t extraBytes = 0;
		while (header & 0b10000000) {
			++extraBytes; // OPTIMIZE: use not and clz here to improve performance
			header <<= 1;
		}
		header >>= extraBytes;
		++ptr;
		// read extra bytes
		if (ptr + extraBytes > end) {
			throw CqlDecodeException(CQL_CODEINFO, "length not enough");
		}
		value_ = header;
		for (std::size_t i = 0; i < extraBytes; ++i) {
			value_ = (value_ << 8) | static_cast<std::uint8_t>(ptr[i]);
		}
		ptr += extraBytes;
	}
}

