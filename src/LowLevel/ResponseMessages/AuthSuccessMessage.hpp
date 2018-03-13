#pragma once
#include "../ProtocolTypes/ProtocolBytes.hpp"
#include "./ResponseMessageBase.hpp"

namespace cql {
	/** Indicates the success of the authentication phase */
	class AuthSuccessMessage : public ResponseMessageBase {
	public:
		using ResponseMessageBase::freeResources;

		/** For Object */
		void reset(MessageHeader&& header);

		/** Get description of this message */
		std::string toString() const override;

		/** Decode message body from binary data */
		void decodeBody(const ConnectionInfo& info, seastar::temporary_buffer<char>&& buffer) override;

		/** Final information from the server that client may require to finish the authentication process */
		const ProtocolBytes& getToken() const& { return token_; }
		ProtocolBytes& getToken() & { return token_; }

		/** Constructor */
		AuthSuccessMessage();

	private:
		ProtocolBytes token_;
	};
}

