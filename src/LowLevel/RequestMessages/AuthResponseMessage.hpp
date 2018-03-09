#pragma once
#include "../ProtocolTypes/ProtocolBytes.hpp"
#include "./RequestMessageBase.hpp"

namespace cql {
	/**
	 * Answers a server authentication challenge
	 * The response to a AUTH_RESPONSE is either a follow-up AUTH_CHALLENGE message,
	 * an AUTH_SUCCESS message or an ERROR message.
	 */
	class AuthResponseMessage : public RequestMessageBase {
	public:
		using RequestMessageBase::freeResources;
		
		/** For Object */
		void reset(MessageHeader&& header);

		/** Get description of this message */
		seastar::sstring toString() const override;

		/** Encode message body to binary data */
		void encodeBody(const ConnectionInfo& info, seastar::sstring& data) const override;

		/** The detail of what this token contains depends on the actual authenticator used */
		const ProtocolBytes& getToken() const& { return token_; }
		ProtocolBytes& getToken() & { return token_; }

		/** Constructor */
		AuthResponseMessage();

	private:
		ProtocolBytes token_;
	};
}

