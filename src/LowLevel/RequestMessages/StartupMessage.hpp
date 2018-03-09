#pragma once
#include "../ProtocolTypes/ProtocolStringMap.hpp"
#include "./RequestMessageBase.hpp"

namespace cql {
	/**
	 * Initialize the connection
	 * The server will respond by either a READY message or an AUTHENTICATE message.
	 */
	class StartupMessage : public RequestMessageBase {
	public:
		using RequestMessageBase::freeResources;

		/** For Object */
		void reset(MessageHeader&& header);

		/** Get description of this message */
		seastar::sstring toString() const override;

		/** Encode message body to binary data */
		void encodeBody(const ConnectionInfo& info, seastar::sstring& data) const override;

		/** Set the compression algorithm to use, support "lz4", "snappy", or nullptr */
		void setCompression(const char* algorithm);

		/** Constructor */
		StartupMessage();

	private:
		ProtocolStringMap options_;
	};
}

