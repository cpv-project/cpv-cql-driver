#pragma once
#include "../ProtocolTypes/ProtocolBytes.hpp"
#include "./ResponseMessageBase.hpp"

namespace cql {
	/**
	 * A server authentication challenge
	 * Clients are expected to answer the server challenge with an AUTH_RESPONSE message.
	 */
	class AuthChallengeMessage : public ResponseMessageBase {
	public:
		using ResponseMessageBase::freeResources;

		/** For Object */
		void reset(MessageHeader&& header);

		/** Get description of this message */
		std::string toString() const override;

		/** Decode message body from binary data */
		void decodeBody(const ConnectionInfo& info, seastar::temporary_buffer<char>&& buffer) override;

		/** The details of what this token contains depends on the actual authenticator used */
		const ProtocolBytes& getToken() const& { return token_; }
		ProtocolBytes& getToken() & { return token_; }

		/** Constructor */
		AuthChallengeMessage();

	private:
		ProtocolBytes token_;
	};
}

