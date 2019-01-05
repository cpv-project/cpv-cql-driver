#include <array>
#include <seastar/core/byteorder.hh>
#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <CQLDriver/Common/CommonDefinitions.hpp>
#include "./ProtocolVariableIntegerBase.hpp"

namespace cql {
	namespace {
		/** The first byte use to indicate how many extra bytes followed */
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

	/** Encode to binary data */
	void ProtocolVariableIntegerBase::encode(std::string& data) const {
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

	/** Decode from binary data */
	void ProtocolVariableIntegerBase::decode(const char*& ptr, const char* end) {
		if (CQL_UNLIKELY(ptr + sizeof(char) > end)) {
			throw DecodeException(CQL_CODEINFO, "length not enough");
		}
		// read header
		std::uint8_t header = static_cast<std::uint8_t>(*ptr);
		std::size_t extraBytes = 0;
#if defined(__i386__) || defined(__amd64__)
		static const constexpr std::size_t bitsDistance = (sizeof(unsigned int) - 1) * 8;
		static_assert(bitsDistance > 0, "ensure ~(header << bitsDistance) != 0");
		extraBytes = __builtin_clz(~(static_cast<unsigned int>(header) << bitsDistance));
		header = static_cast<uint8_t>(header << extraBytes) >> extraBytes;
#else
		while (header & 0b10000000) {
			++extraBytes;
			header <<= 1;
		}
		header >>= extraBytes;
#endif
		++ptr;
		// read extra bytes
		if (CQL_UNLIKELY(ptr + extraBytes > end)) {
			throw DecodeException(CQL_CODEINFO, "length not enough");
		}
		value_ = header;
		for (std::size_t i = 0; i < extraBytes; ++i) {
			value_ = (value_ << 8) | static_cast<std::uint8_t>(ptr[i]);
		}
		ptr += extraBytes;
	}
}

