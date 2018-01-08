#pragma once
#include <core/sstring.hh>
#include <core/temporary_buffer.hh>
#include "CqlLowLevelDefinitions.hpp"
#include "CqlConnectionInfo.hpp"
#include "ProtocolTypes/CqlProtocolByte.hpp"
#include "ProtocolTypes/CqlProtocolShort.hpp"
#include "ProtocolTypes/CqlProtocolInt.hpp"

namespace cql {
	/**
	 * The message header (aka frame header)
	 * Also see the comments in CqlMessageBase.hpp for the encode and decode instruction.
	 */
	class CqlMessageHeader {
	public:
		using VersionType = CqlProtocolByte;
		using FlagsType = CqlProtocolByte;
		using StreamIdType = CqlProtocolShort;
		using OpCodeType = CqlProtocolByte;
		using BodyLengthType = CqlProtocolInt;

		/** Encode message header to binary data */
		void encodeHeaderPre(const CqlConnectionInfo& info, seastar::sstring& data) const;

		/** Update body length in binary data */
		void encodeHeaderPost(const CqlConnectionInfo& info, seastar::sstring& data) const;

		/** Decode message header from binary data */
		void decodeHeader(const CqlConnectionInfo& info, const seastar::temporary_buffer<char>& data);

		/** Get the stream id of message */
		StreamIdType getStreamId() const;

		/**
		 * Get the opcode of message
		 * Also see the CqlMessageType in CqlDefinitions.hpp
		 */
		OpCodeType getOpCode() const;

		/**
		 * Get the length of message body (aka frame payload)
		 * Only available for response message, for request message it always return 0.
		 */
		BodyLengthType getBodyLength() const;

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
		void setStreamId(const StreamIdType& streamId);

		/**
		 * Set the opcode of message
		 * Also see the CqlMessageType in CqlDefinitions.hpp
		 */
		void setOpCode(const OpCodeType& opCode);

		/** Constructor */
		CqlMessageHeader();

	private:
		StreamIdType streamId_;
		OpCodeType opCode_;
		BodyLengthType bodyLength_;
	};
}

