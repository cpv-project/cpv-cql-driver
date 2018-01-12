#pragma once
#include "../CqlMessageBase.hpp"

namespace cql {
	/** The base class of request message */
	class CqlRequestMessageBase : public CqlMessageBase {
	public:
		/** Encode message body to binary data */
		virtual void encodeBody(const CqlConnectionInfo& info, seastar::sstring& data) const = 0;
	};
}

