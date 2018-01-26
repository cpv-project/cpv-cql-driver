#include <type_traits>
#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include <CqlDriver/Common/Exceptions/CqlNotImplementedException.hpp>
#include "CqlMessageHeader.hpp"

namespace cql {
	/** Encode message header to binary data */
	void CqlMessageHeader::encodeHeaderPre(
		const CqlConnectionInfo& info, seastar::sstring& data) const {
		if (!data.empty()) {
			throw CqlLogicException(CQL_CODEINFO, "data should be empty");
		}
		// assume it's version 4
		// direction should always be request
		CqlProtocolByte version(info.getVersion() | static_cast<std::size_t>(direction_));
		version.encode(data);
		flags_.encode(data);
		streamId_.encode(data);
		opCode_.encode(data);
		bodyLength_.encode(data);
	}

	/** Update body length in binary data */
	void CqlMessageHeader::encodeHeaderPost(
		const CqlConnectionInfo& info, seastar::sstring& data) const {
		// assume it's version 4
		// direction should always be response
		auto headerSize = info.getHeaderSize();
		if (data.size() < headerSize) {
			throw CqlLogicException(CQL_CODEINFO, "please call encodeHeaderPre first");
		}
		static thread_local decltype(bodyLength_) bodyLength;
		static thread_local seastar::sstring bodyLengthData;
		bodyLength.set(data.size() - headerSize);
		bodyLengthData.resize(0);
		bodyLength.encode(bodyLengthData);
		std::memcpy(
			data.data() + headerSize - bodyLengthData.size(),
			bodyLengthData.data(),
			bodyLengthData.size());
	}

	/** Decode message header from binary data */
	void CqlMessageHeader::decodeHeader(
		const CqlConnectionInfo& info, seastar::temporary_buffer<char>&& buffer) {
		// assume it's version 4
		const char* ptr = buffer.begin();
		const char* end = buffer.end();
		CqlProtocolByte version;
		version.decode(ptr, end);
		flags_.decode(ptr, end);
		streamId_.decode(ptr, end);
		opCode_.decode(ptr, end);
		bodyLength_.decode(ptr, end);
		direction_ = static_cast<CqlMessageDirection>(
			version.get() & static_cast<std::size_t>(CqlMessageDirection::Mask_));
	}

	/** Get the flags of message */
	CqlMessageHeaderFlags CqlMessageHeader::getFlags() const {
		return static_cast<CqlMessageHeaderFlags>(flags_.get());
	}

	/** Get the stream id of message */
	std::size_t CqlMessageHeader::getStreamId() const {
		return streamId_.get();
	}

	/** Get the opcode of message */
	CqlMessageType CqlMessageHeader::getOpCode() const {
		return static_cast<CqlMessageType>(opCode_.get());
	}

	/** Get the direction of message, it depends on opcode */
	CqlMessageDirection CqlMessageHeader::getDirection() const {
		return direction_;
	}

	/** Get the length of message body (aka frame payload) */
	std::size_t CqlMessageHeader::getBodyLength() const {
		auto bodyLength = bodyLength_.get();
		if (std::is_signed<decltype(bodyLength)>::value && bodyLength < 0) {
			throw CqlLogicException(CQL_CODEINFO, "body length < 0");
		}
		return static_cast<std::size_t>(bodyLength);
	}

	/** Set the flags of message */
	void CqlMessageHeader::setFlags(CqlMessageHeaderFlags flags) {
		flags_.set(static_cast<decltype(flags_.get())>(flags));
	}

	/** Set the stream id of message */
	void CqlMessageHeader::setStreamId(std::size_t streamId) {
		streamId_.set(static_cast<decltype(streamId_.get())>(streamId));
		if (streamId_.get() != streamId) {
			throw CqlLogicException(CQL_CODEINFO, "stream id overflow");
		}
	}

	/** Set the opcode and direction of message */
	void CqlMessageHeader::setOpCode(CqlMessageType opCode, CqlMessageDirection direction) {
		opCode_.set(static_cast<decltype(opCode_.get())>(opCode));
		direction_ = direction;
	}

	/** Constructor */
	CqlMessageHeader::CqlMessageHeader() :
		flags_(),
		streamId_(),
		opCode_(),
		direction_(),
		bodyLength_() { }
}

