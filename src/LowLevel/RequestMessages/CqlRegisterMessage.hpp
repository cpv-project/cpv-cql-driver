#pragma once
#include "CqlRequestMessageBase.hpp"

namespace cql {
	/**
	 * Register this connection to receive some types of events
	 * The response to a REGISTER message will be a READY message.
	 */
	class CqlRegisterMessage : public CqlRequestMessageBase {
	public:
		/** Encode message body to binary data */
		void encodeBody(const CqlConnectionInfo& info, seastar::sstring& data) const override;
	};
}

