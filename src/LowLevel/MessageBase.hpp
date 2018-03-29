#pragma once
#include <CQLDriver/Common/Utility/Object.hpp>
#include "./LowLevelDefinitions.hpp"
#include "./MessageHeader.hpp"
#include "./ConnectionInfo.hpp"

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
	class MessageBase {
	public:
		/** Get the message header */
		const MessageHeader& getHeader() const&;

		/** Get the mutable message header */
		MessageHeader& getHeader() &;

		/** Get description of this message */
		virtual std::string toString() const = 0;

		/** Constructor */
		MessageBase(); 

		/** Disable copy and move */
		MessageBase(const MessageBase&) = delete;
		MessageBase(MessageBase&&) = delete;
		MessageBase& operator=(const MessageBase&) = delete;
		MessageBase& operator=(MessageBase&&) = delete;

		/** Virtual destructor */
		virtual ~MessageBase() = default;

	protected:
		/** For Object support */
		static void freeResources();

		/** For Object support */
		void reset(MessageHeader&& header);

	protected:
		MessageHeader header_;
	};
}

