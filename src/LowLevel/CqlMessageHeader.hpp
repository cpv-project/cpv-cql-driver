#pragma once
#include "CqlConnectionInfo.hpp"
#include <core/sstring.hh>

namespace cql {
	/**
	 * The message header (aka frame header)
	 */
	class CqlMessageHeader {
	public:
		/** Encode message header to binary data */
		void encodeHeader(const CqlConnectionInfo& info, seastar::sstring& data) const;

		/** Decode message header to binary data */
		void decodeHeader(const CqlConnectionInfo& info, const seastar::sstring& data);

		/** Constructor */
		CqlMessageHeader();

	private:
		// TODO
	};
}

