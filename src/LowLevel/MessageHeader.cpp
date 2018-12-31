#include <type_traits>
#include <CQLDriver/Common/Exceptions/LogicException.hpp>
#include <CQLDriver/Common/Exceptions/NotImplementedException.hpp>
#include "./MessageHeader.hpp"

namespace cql {
	/** Encode message header to binary data */
	void MessageHeader::encodeHeaderPre(
		const ConnectionInfo& info, std::string& data) const {
		if (!data.empty()) {
			throw LogicException(CQL_CODEINFO, "data should be empty");
		}
		// assume it's version 4
		// direction should always be request
		ProtocolByte version(info.getVersion() | static_cast<std::size_t>(direction_));
		version.encode(data);
		flags_.encode(data);
		streamId_.encode(data);
		opCode_.encode(data);
		bodyLength_.encode(data);
	}

	/** Update body length in binary data */
	void MessageHeader::encodeHeaderPost(
		const ConnectionInfo& info, std::string& data) const {
		// assume it's version 4
		// direction should always be response
		auto headerSize = info.getHeaderSize();
		if (data.size() < headerSize) {
			throw LogicException(CQL_CODEINFO, "please call encodeHeaderPre first");
		}
		static thread_local decltype(bodyLength_) bodyLength;
		static thread_local std::string bodyLengthData;
		bodyLength.set(data.size() - headerSize);
		bodyLengthData.resize(0);
		bodyLength.encode(bodyLengthData);
		std::memcpy(
			&data[headerSize - bodyLengthData.size()],
			bodyLengthData.data(),
			bodyLengthData.size());
	}

	/** Decode message header from binary data */
	void MessageHeader::decodeHeader(
		const ConnectionInfo&, seastar::temporary_buffer<char>&& buffer) {
		// assume it's version 4
		const char* ptr = buffer.begin();
		const char* end = buffer.end();
		ProtocolByte version;
		version.decode(ptr, end);
		flags_.decode(ptr, end);
		streamId_.decode(ptr, end);
		opCode_.decode(ptr, end);
		bodyLength_.decode(ptr, end);
		direction_ = static_cast<MessageDirection>(
			version.get() & static_cast<std::size_t>(MessageDirection::Mask_));
	}

	/** Get the flags of message */
	MessageHeaderFlags MessageHeader::getFlags() const {
		return static_cast<MessageHeaderFlags>(flags_.get());
	}

	/** Get the stream id of message */
	std::size_t MessageHeader::getStreamId() const {
		return streamId_.get();
	}

	/** Get the opcode of message */
	MessageType MessageHeader::getOpCode() const {
		return static_cast<MessageType>(opCode_.get());
	}

	/** Get the direction of message, it depends on opcode */
	MessageDirection MessageHeader::getDirection() const {
		return direction_;
	}

	/** Get the length of message body (aka frame payload) */
	std::size_t MessageHeader::getBodyLength() const {
		auto bodyLength = bodyLength_.get();
		if (std::is_signed<decltype(bodyLength)>::value && bodyLength < 0) {
			throw LogicException(CQL_CODEINFO, "body length < 0");
		}
		return static_cast<std::size_t>(bodyLength);
	}

	/** Set the flags of message */
	void MessageHeader::setFlags(MessageHeaderFlags flags) {
		flags_.set(static_cast<decltype(flags_.get())>(flags));
	}

	/** Set the stream id of message */
	void MessageHeader::setStreamId(std::size_t streamId) {
		streamId_.set(static_cast<decltype(streamId_.get())>(streamId));
		if (streamId_.get() != streamId) {
			throw LogicException(CQL_CODEINFO, "stream id overflow");
		}
	}

	/** Set the opcode and direction of message */
	void MessageHeader::setOpCode(MessageType opCode, MessageDirection direction) {
		opCode_.set(static_cast<decltype(opCode_.get())>(opCode));
		direction_ = direction;
	}

	/** Constructor */
	MessageHeader::MessageHeader() :
		flags_(),
		streamId_(),
		opCode_(),
		direction_(),
		bodyLength_() { }
}

