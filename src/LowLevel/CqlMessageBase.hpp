#pragma once
#include <CqlDriver/Utility/CqlObject.hpp>
#include "CqlDefinitions.hpp"
#include "CqlMessageHeader.hpp"
#include "CqlConnectionInfo.hpp"

namespace cql {
	/**
	 * The base class of message
	 * How the message encoded and decoded:
	 * Request Encode:
	 * - construct message with empty header (createRequestMessage)
	 * - encode header (encodeHeaderPre)
	 * - encode body (encodeBody)
	 * - update length in header (encodeHeaderPost)
	 * - send header AND body
	 * Response Decode:
	 * - receive header
	 * - decode header (decodeHeader)
	 * - construct message with decoded header (createResponseMessage)
	 * - receive body
	 * - decode body (decodeBody)
	 */
	class CqlMessageBase {
		/** Get the direction of message, either request or response */
		CqlMessageDirection getDirection() const;

		/** Get the type of message */
		CqlMessageType getType() const;

		/** Get the message header */
		const CqlMessageHeader& getHeader() const&;

		/** Encode message body to binary data */
		virtual void encodeBody(const CqlConnectionInfo& info, seastar::sstring& data) const = 0;

		/** Decode message body from binary data */
		virtual void decodeBody(const CqlConnectionInfo& info, const seastar::temporary_buffer<char>& data) = 0;

		/** Virtual destructor */
		virtual ~CqlMessageBase();

		/** Constructor */
		CqlMessageBase(CqlObject<CqlMessageHeader>&& header);
	};
}

