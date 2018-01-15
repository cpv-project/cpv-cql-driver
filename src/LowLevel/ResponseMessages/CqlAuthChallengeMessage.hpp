#pragma once
#include "CqlResponseMessageBase.hpp"

namespace cql {
	/**
	 * A server authentication challenge
	 * Clients are expected to answer the server challenge with an AUTH_RESPONSE message.
	 */
	class CqlAuthChallengeMessage : public CqlResponseMessageBase {
	public:
		using CqlResponseMessageBase::freeResources;
		using CqlResponseMessageBase::reset;

		/** Decode message body from binary data */
		void decodeBody(const CqlConnectionInfo& info, const seastar::temporary_buffer<char>& data) override;
	};
}

