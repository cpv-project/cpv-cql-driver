#pragma once
#include "../ProtocolTypes/ProtocolString.hpp"
#include "./ResponseMessageBase.hpp"

namespace cql {
	/**
	 * Indicates that the server requires authentication, and which authentication mechaism to use
	 * This message will be sent following a STARTUP message if authentication is required
	 * and must be answered by a AUTH_RESPONSE message from the client.
	 */
	class AuthenticateMessage : public ResponseMessageBase {
	public:
		using ResponseMessageBase::freeResources;

		/** For Object */
		void reset(MessageHeader&& header);

		/** Get description of this message */
		std::string toString() const override;

		/** Decode message body from binary data */
		void decodeBody(const ConnectionInfo& info, seastar::temporary_buffer<char>&& buffer) override;

		/** The full class name of the IAuthenticator in use */
		const ProtocolString& getAuthenticatorClass() const& { return authenticatorClass_; }
		ProtocolString& getAuthenticatorClass() & { return authenticatorClass_; }

		/** Constructor */
		AuthenticateMessage();

	private:
		ProtocolString authenticatorClass_;
	};
}

