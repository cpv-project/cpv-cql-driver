#pragma once
#include <core/sstring.hh>
#include <core/temporary_buffer.hh>
#include "./LowLevelDefinitions.hpp"
#include "./ConnectionInfo.hpp"
#include "./ProtocolTypes/ProtocolByte.hpp"
#include "./ProtocolTypes/ProtocolShort.hpp"
#include "./ProtocolTypes/ProtocolInt.hpp"

namespace cql {
	/**
	 * The message header (aka frame header)
	 * Also see the comments in MessageBase.hpp for the encode and decode instruction.
	 */
	class MessageHeader {
	public:
		/** Encode message header to binary data */
		void encodeHeaderPre(const ConnectionInfo& info, seastar::sstring& data) const;

		/** Update body length in binary data */
		void encodeHeaderPost(const ConnectionInfo& info, seastar::sstring& data) const;

		/** Decode message header from binary data */
		void decodeHeader(const ConnectionInfo& info, seastar::temporary_buffer<char>&& buffer);

		/**
		 * Get the flags of message
		 * Also see the MessageHeaderFlags in LowLevelDefinitions.hpp
		 */
		MessageHeaderFlags getFlags() const;

		/** Get the stream id of message */
		std::size_t getStreamId() const;

		/**
		 * Get the opcode of message
		 * Also see the MessageType in LowLevelDefinitions.hpp
		 */
		MessageType getOpCode() const;

		/** Get the direction of message, it depends on opcode */
		MessageDirection getDirection() const;

		/**
		 * Get the length of message body (aka frame payload)
		 * Only available for response message, for request message it always return 0.
		 */
		std::size_t getBodyLength() const;

		/**
		 * Set the flags of message
		 */
		void setFlags(MessageHeaderFlags flags);

		/**
		 * Set the stream id of message
		 * Quoted from protocol definition:
		 * - When sending request message, stream id must be a non-negative value.
		 * - Negative stream id are reserved for streams initiated by the server,
		 * - like all EVENT messages have a stream id of -1.
		 * - If a client sends a request message with the stream id X,
		 * - it's graranteed that the stream id of the response to that message will be X.
		 * Note:
		 * - It's weird that stream id is defined as [short] which is a unsigned integer,
		 * - the -1 mentioned before is actually 0xffff,
		 * - and the maximum stream id the client can send is 0x7fff(32767).
		 */
		void setStreamId(std::size_t streamId);

		/**
		 * Set the opcode and direction of message
		 * Also see the MessageType in Definitions.hpp
		 */
		void setOpCode(MessageType opCode, MessageDirection direction);

		/** Constructor */
		MessageHeader();

	private:
		ProtocolByte flags_;
		ProtocolShort streamId_;
		ProtocolByte opCode_;
		MessageDirection direction_;
		ProtocolInt bodyLength_;
	};
}

