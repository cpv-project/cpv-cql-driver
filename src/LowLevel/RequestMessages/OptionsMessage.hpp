#pragma once
#include "./RequestMessageBase.hpp"

namespace cql {
	/**
	 * Asks the server to return which STARTUP options are supported
	 * The server will respond with a SUPPORTED message.
	 */
	class OptionsMessage : public RequestMessageBase {
	public:
		using RequestMessageBase::freeResources;
		using RequestMessageBase::reset;

		/** Get description of this message */
		seastar::sstring toString() const override;

		/** Encode message body to binary data */
		void encodeBody(const ConnectionInfo& info, seastar::sstring& data) const override;
	};
}

