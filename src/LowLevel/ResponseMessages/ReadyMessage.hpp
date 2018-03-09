#pragma once
#include "./ResponseMessageBase.hpp"

namespace cql {
	/**
	 * Indicates that the server is ready to process queries
	 * This message will be sent by the server either after a STARTUP message if no authentication is required.
	 * (if authentication is required, the server indicates readiness by sending a AUTH_RESPONSE message)
	 */
	class ReadyMessage : public ResponseMessageBase {
	public:
		using ResponseMessageBase::freeResources;
		using ResponseMessageBase::reset;

		/** Get description of this message */
		seastar::sstring toString() const override;

		/** Decode message body from binary data */
		void decodeBody(const ConnectionInfo& info, seastar::temporary_buffer<char>&& buffer) override;
	};
}

