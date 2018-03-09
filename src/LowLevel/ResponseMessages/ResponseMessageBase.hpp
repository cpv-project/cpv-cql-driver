#pragma once
#include "../MessageBase.hpp"

namespace cql {
	/** The base class of response message */
	class ResponseMessageBase : public MessageBase {
	public:
		/** Decode message body from binary data */
		virtual void decodeBody(const ConnectionInfo& info, seastar::temporary_buffer<char>&& buffer) = 0;
	};
}

