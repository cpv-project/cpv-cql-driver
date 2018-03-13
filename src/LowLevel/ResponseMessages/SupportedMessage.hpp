#pragma once
#include "../ProtocolTypes/ProtocolStringMultiMap.hpp"
#include "./ResponseMessageBase.hpp"

namespace cql {
	/**
	 * Indicates which startup options are supported by the server
	 * This message comes as a response to an OPTIONS message.
	 */
	class SupportedMessage : public ResponseMessageBase {
	public:
		using ResponseMessageBase::freeResources;

		/** For Object */
		void reset(MessageHeader&& header);

		/** Decode message body from binary data */
		void decodeBody(const ConnectionInfo& info, seastar::temporary_buffer<char>&& buffer) override;

		/** Get description of this message */
		std::string toString() const override;

		/** The supported STARTUP options */
		const ProtocolStringMultiMap& getOptions() const& { return options_; }
		ProtocolStringMultiMap& getOptions() & { return options_; }

		/** Constructor */
		SupportedMessage();

	private:
		ProtocolStringMultiMap options_;
	};
}

