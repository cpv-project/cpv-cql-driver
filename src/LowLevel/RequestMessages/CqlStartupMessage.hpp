#pragma once
#include "CqlRequestMessageBase.hpp"

namespace cql {
	/**
	 * Initialize the connection
	 * The server will respond by either a READY message or an AUTHENTICATE message.
	 */
	class CqlStartupMessage : public CqlRequestMessageBase {
	public:
		using CqlRequestMessageBase::freeResources;
		using CqlRequestMessageBase::reset;

		/** Encode message body to binary data */
		void encodeBody(const CqlConnectionInfo& info, seastar::sstring& data) const override;
	};
}

