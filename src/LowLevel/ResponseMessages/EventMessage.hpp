#pragma once
#include "../ProtocolTypes/ProtocolString.hpp"
#include "./ResponseMessageBase.hpp"

namespace cql {
	/**
	 * An event pushed by the server
	 * A client will only receive events for the types it has REGISTERed to.
	 */
	class EventMessage : public ResponseMessageBase {
	public:
		using ResponseMessageBase::freeResources;
		
		/** For Object */
		void reset(MessageHeader&& header);

		/** Get description of this message */
		std::string toString() const override;

		/** Decode message body from binary data */
		void decodeBody(const ConnectionInfo& info, seastar::temporary_buffer<char>&& buffer) override;

		/** The event type */
		const ProtocolString& getType() const& { return type_; }
		ProtocolString& getType() & { return type_; }

		/** The event data, depends on the event type */
		const std::string& getData() const& { return data_; }
		std::string& getData() & { return data_; }

		/** Constructor */
		EventMessage();

	private:
		ProtocolString type_;
		std::string data_;
	};
}

