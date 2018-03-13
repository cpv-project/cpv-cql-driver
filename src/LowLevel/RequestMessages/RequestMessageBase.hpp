#pragma once
#include "../MessageBase.hpp"

namespace cql {
	/** The base class of request message */
	class RequestMessageBase : public MessageBase {
	public:
		/** Encode message body to binary data */
		virtual void encodeBody(const ConnectionInfo& info, std::string& data) const = 0;
	};
}

