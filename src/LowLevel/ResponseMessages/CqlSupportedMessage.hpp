#pragma once
#include "CqlResponseMessageBase.hpp"

namespace cql {
	/**
	 * Indicates which startup options are supported by the server
	 * This message comes as a response to an OPTIONS message.
	 */
	class CqlSupportedMessage : public CqlResponseMessageBase {
	public:
		using CqlResponseMessageBase::freeResources;
		using CqlResponseMessageBase::reset;

		/** Decode message body from binary data */
		void decodeBody(const CqlConnectionInfo& info, const char*& ptr, const char* end) override;
	};
}

