#pragma once
#include "CqlResponseMessageBase.hpp"

namespace cql {
	/**
	 * An event pushed by the server
	 * A client will only receive events for the types it has REGISTERed to.
	 */
	class CqlEventMessage : public CqlResponseMessageBase {
	public:
		using CqlResponseMessageBase::freeResources;
		using CqlResponseMessageBase::reset;

		/** Decode message body from binary data */
		void decodeBody(const CqlConnectionInfo& info, const char*& ptr, const char* end) override;
	};
}

