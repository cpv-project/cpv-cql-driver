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

		/** Get the message header */
		CqlMessageHeader& getHeader() &;

		/** Get description of this message */
		virtual seastar::sstring toString() const = 0;

		/** Constructor */
		CqlMessageBase(); 

		/** Disable copy and move */
		CqlMessageBase(const CqlMessageBase&) = delete;
		CqlMessageBase(CqlMessageBase&&) = delete;
		CqlMessageBase& operator=(const CqlMessageBase&) = delete;
		CqlMessageBase& operator=(CqlMessageBase&&) = delete;

		/** Virtual destructor */
		virtual ~CqlMessageBase() = default;

	protected:
		// cppcheck-suppress functionStatic
		/** For CqlObject support */
		void freeResources();

		/** For CqlObject support */
		void reset(CqlMessageHeader&& header);

	protected:
		CqlMessageHeader header_;
	};
}

