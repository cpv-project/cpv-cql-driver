#pragma once
#include "../ProtocolTypes/CqlProtocolStringMap.hpp"
#include "CqlRequestMessageBase.hpp"

namespace cql {
	/**
	 * Initialize the connection
	 * The server will respond by either a READY message or an AUTHENTICATE message.
	 */
	class CqlStartupMessage : public CqlRequestMessageBase {
	public:
		using CqlRequestMessageBase::freeResources;

		/** For CqlObject */
		void reset(CqlMessageHeader&& header);

		/** Encode message body to binary data */
		void encodeBody(const CqlConnectionInfo& info, seastar::sstring& data) const override;

		/** Set the compression algorithm to use, support "lz4", "snappy", or nullptr */
		void setCompression(const char* algorithm);

		/** Constructor */
		CqlStartupMessage();

	private:
		CqlProtocolStringMap options_;
	};
}

