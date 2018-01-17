#pragma once
#include "../ProtocolTypes/CqlProtocolStringList.hpp"
#include "CqlRequestMessageBase.hpp"

namespace cql {
	/**
	 * Register this connection to receive some types of events
	 * The response to a REGISTER message will be a READY message.
	 */
	class CqlRegisterMessage : public CqlRequestMessageBase {
	public:
		using CqlRequestMessageBase::freeResources;

		/** For CqlObject */
		void reset(CqlMessageHeader&& header);

		/** Encode message body to binary data */
		void encodeBody(const CqlConnectionInfo& info, seastar::sstring& data) const override;

		/** Get the event types to register for */
		const CqlProtocolStringList& getEvents() const& { return events_; }
		CqlProtocolStringList& getEvents() & { return events_; }

		/** Constructor */
		CqlRegisterMessage();

	private:
		CqlProtocolStringList events_;
	};
}

