#pragma once
#include "../ProtocolTypes/CqlProtocolBytes.hpp"
#include "CqlRequestMessageBase.hpp"

namespace cql {
	/**
	 * Answers a server authentication challenge
	 * The response to a AUTH_RESPONSE is either a follow-up AUTH_CHALLENGE message,
	 * an AUTH_SUCCESS message or an ERROR message.
	 */
	class CqlAuthResponseMessage : public CqlRequestMessageBase {
	public:
		using CqlRequestMessageBase::freeResources;
		
		/** For CqlObject */
		void reset(CqlMessageHeader&& header);

		/** Encode message body to binary data */
		void encodeBody(const CqlConnectionInfo& info, seastar::sstring& data) const override;

		/** The detail of what this token contains depends on the actual authenticator used */
		const CqlProtocolBytes& getToken() const& { return token_; }
		CqlProtocolBytes& getToken() & { return token_; }
		void setToken(CqlProtocolBytes&& token) { token_ = std::move(token); }

		/** Constructor */
		CqlAuthResponseMessage();

	private:
		CqlProtocolBytes token_;
	};
}

