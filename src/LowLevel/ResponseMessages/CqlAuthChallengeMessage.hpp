#pragma once
#include "../ProtocolTypes/CqlProtocolBytes.hpp"
#include "CqlResponseMessageBase.hpp"

namespace cql {
	/**
	 * A server authentication challenge
	 * Clients are expected to answer the server challenge with an AUTH_RESPONSE message.
	 */
	class CqlAuthChallengeMessage : public CqlResponseMessageBase {
	public:
		using CqlResponseMessageBase::freeResources;

		/** For CqlObject */
		void reset(CqlMessageHeader&& header);

		/** Get description of this message */
		seastar::sstring toString() const override;

		/** Decode message body from binary data */
		void decodeBody(const CqlConnectionInfo& info, seastar::temporary_buffer<char>&& buffer) override;

		/** The details of what this token contains depends on the actual authenticator used */
		const CqlProtocolBytes& getToken() const& { return token_; }
		CqlProtocolBytes& getToken() & { return token_; }

		/** Constructor */
		CqlAuthChallengeMessage();

	private:
		CqlProtocolBytes token_;
	};
}

