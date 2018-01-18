#pragma once
#include "../ProtocolTypes/CqlProtocolStringMultiMap.hpp"
#include "CqlResponseMessageBase.hpp"

namespace cql {
	/**
	 * Indicates which startup options are supported by the server
	 * This message comes as a response to an OPTIONS message.
	 */
	class CqlSupportedMessage : public CqlResponseMessageBase {
	public:
		using CqlResponseMessageBase::freeResources;

		/** For CqlObject */
		void reset(CqlMessageHeader&& header);

		/** Decode message body from binary data */
		void decodeBody(const CqlConnectionInfo& info, const char*& ptr, const char* end) override;

		/** The supported STARTUP options */
		const CqlProtocolStringMultiMap& getOptions() const& { return options_; }
		CqlProtocolStringMultiMap& getOptions() & { return options_; }

		/** Constructor */
		CqlSupportedMessage();

	private:
		CqlProtocolStringMultiMap options_;
	};
}

