#pragma once
#include "../ProtocolTypes/CqlProtocolBytes.hpp"
#include "CqlResponseMessageBase.hpp"

namespace cql {
	/** Indicates the success of the authentication phase */
	class CqlAuthSuccessMessage : public CqlResponseMessageBase {
	public:
		using CqlResponseMessageBase::freeResources;

		/** For CqlObject */
		void reset(CqlMessageHeader&& header);

		/** Decode message body from binary data */
		void decodeBody(const CqlConnectionInfo& info, const char*& ptr, const char* end) override;

		/** Final information from the server that client may require to finish the authentication process */
		const CqlProtocolBytes& getToken() const& { return token_; }
		CqlProtocolBytes& getToken() & { return token_; }

		/** Constructor */
		CqlAuthSuccessMessage();

	private:
		CqlProtocolBytes token_;
	};
}

