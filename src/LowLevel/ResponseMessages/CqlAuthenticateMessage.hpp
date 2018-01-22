#pragma once
#include "../ProtocolTypes/CqlProtocolString.hpp"
#include "CqlResponseMessageBase.hpp"

namespace cql {
	/**
	 * Indicates that the server requires authentication, and which authentication mechaism to use
	 * This message will be sent following a STARTUP message if authentication is required
	 * and must be answered by a AUTH_RESPONSE message from the client.
	 */
	class CqlAuthenticateMessage : public CqlResponseMessageBase {
	public:
		using CqlResponseMessageBase::freeResources;

		/** For CqlObject */
		void reset(CqlMessageHeader&& header);

		/** Get description of this message */
		seastar::sstring toString() const override;

		/** Decode message body from binary data */
		void decodeBody(const CqlConnectionInfo& info, const char*& ptr, const char* end) override;

		/** The full class name of the IAuthenticator in use */
		const CqlProtocolString& getAuthenticatorClass() const& { return authenticatorClass_; }
		CqlProtocolString& getAuthenticatorClass() & { return authenticatorClass_; }

		/** Constructor */
		CqlAuthenticateMessage();

	private:
		CqlProtocolString authenticatorClass_;
	};
}

