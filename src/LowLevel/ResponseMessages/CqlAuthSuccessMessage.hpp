#pragma once
#include "CqlResponseMessageBase.hpp"

namespace cql {
	/** Indicates the success of the authentication phase */
	class CqlAuthSuccessMessage : public CqlResponseMessageBase {
	public:
		using CqlResponseMessageBase::freeResources;
		using CqlResponseMessageBase::reset;

		/** Decode message body from binary data */
		void decodeBody(const CqlConnectionInfo& info, const char*& ptr, const char* end) override;
	};
}

