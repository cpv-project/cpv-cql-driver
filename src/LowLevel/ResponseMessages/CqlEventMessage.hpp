#pragma once
#include "../ProtocolTypes/CqlProtocolString.hpp"
#include "CqlResponseMessageBase.hpp"

namespace cql {
	/**
	 * An event pushed by the server
	 * A client will only receive events for the types it has REGISTERed to.
	 */
	class CqlEventMessage : public CqlResponseMessageBase {
	public:
		using CqlResponseMessageBase::freeResources;
		
		/** For CqlObject */
		void reset(CqlMessageHeader&& header);

		/** Get description of this message */
		seastar::sstring toString() const override;

		/** Decode message body from binary data */
		void decodeBody(const CqlConnectionInfo& info, seastar::temporary_buffer<char>&& buffer) override;

		/** The event type */
		const CqlProtocolString& getType() const& { return type_; }
		CqlProtocolString& getType() & { return type_; }

		/** The event data, depends on the event type */
		const seastar::sstring& getData() const& { return data_; }
		seastar::sstring& getData() & { return data_; }

		/** Constructor */
		CqlEventMessage();

	private:
		CqlProtocolString type_;
		seastar::sstring data_;
	};
}

