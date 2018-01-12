#pragma once
#include <CqlDriver/Common/Utility/CqlObject.hpp>
#include "CqlLowLevelDefinitions.hpp"
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
	public:
		/** Get the message header */
		const CqlMessageHeader& getHeader() const&;

		/** Virtual destructor */
		virtual ~CqlMessageBase() = default;

		/** Constructor */
		CqlMessageBase(CqlMessageHeader&& header); 

	protected:
		CqlMessageHeader header_;
	};
}

