#pragma once
#include "../ProtocolTypes/ProtocolStringList.hpp"
#include "./RequestMessageBase.hpp"

namespace cql {
	/**
	 * Register this connection to receive some types of events
	 * The response to a REGISTER message will be a READY message.
	 */
	class RegisterMessage : public RequestMessageBase {
	public:
		using RequestMessageBase::freeResources;

		/** For Reusable */
		void reset(MessageHeader&& header);

		/** Get description of this message */
		std::string toString() const override;

		/** Encode message body to binary data */
		void encodeBody(const ConnectionInfo& info, std::string& data) const override;

		/** Get the event types to register for */
		const ProtocolStringList& getEvents() const& { return events_; }
		ProtocolStringList& getEvents() & { return events_; }

		/** Constructor */
		RegisterMessage();

	private:
		ProtocolStringList events_;
	};
}

