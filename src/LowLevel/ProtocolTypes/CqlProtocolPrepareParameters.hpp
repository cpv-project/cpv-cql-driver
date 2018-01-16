#pragma once
#include "../CqlLowLevelDefinitions.hpp"
#include "CqlProtocolInt.hpp"
#include "CqlProtocolString.hpp"

namespace cql {
	/**
	 * <flags>[<keyspace>]
	 * [] mean optional, depends on <flags>.
	 * Check native_protocol_v4.spec section 4.1.5.
	 */
	class CqlProtocolPrepareParameters {
	public:
		/** Call setters below will alter flags to indicate which component is included */
		CqlPrepareParametersFlags getFlags() const;
		void resetFlags();

		/** The query should be executed in, supercedes the keyspace that the connection is bound to */
		const seastar::sstring& getKeySpace() const&;
		void setKeySpace(const seastar::sstring& keySpace);
		void setKeySpace(seastar::sstring&& keySpace);

		/** Encode and decode functions */
		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		/** Constructor */
		CqlProtocolPrepareParameters();

	private:
		CqlProtocolInt flags_;
		CqlProtocolString keySpace_;
	};
}

