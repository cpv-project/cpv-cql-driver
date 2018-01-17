#pragma once
#include <CqlDriver/Common/CqlCommonDefinitions.hpp>
#include "../CqlLowLevelDefinitions.hpp"
#include "CqlProtocolConsistency.hpp"
#include "CqlProtocolByte.hpp"
#include "CqlProtocolInt.hpp"
#include "CqlProtocolLong.hpp"
#include "CqlProtocolString.hpp"

namespace cql {
	/**
	 * <consistency><flags>[<serial_consistency>][<timestamp>]
	 * [] mean optional, depends on <flags>.
	 * Check native_protocol_v4.spec section 4.1.7.
	 */
	class CqlProtocolBatchParameters {
	public:
		/** Reset to initial state */
		void reset();

		/** The [consistency] level for the operation */
		CqlConsistencyLevel getConsistency() const;
		void setConsistency(CqlConsistencyLevel consistency);

		/** Call setters below will alter flags to indicate which component is included */
		CqlBatchParametersFlags getFlags() const;

		/**
		 * The [consistency] level for the serial phase of conditional updates,
		 * can only be either SERIAL or LOCAL_SERIAL, the default is SERIAL.
		 */
		CqlConsistencyLevel getSerialConsistency() const;
		void setSerialConsistency(CqlConsistencyLevel serialConsistency);

		/**
		 * This will replace the server side assigned timestamp as default timestamp,
		 * note that a timestamp in the query itself will still override this timestamp.
		 */
		std::uint64_t getDefaultTimestamp() const;
		void setDefaultTimestamp(std::uint64_t timestamp);

		/** Encode and decode functions */
		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		/** Constructor */
		CqlProtocolBatchParameters();

	private:
		CqlProtocolConsistency consistency_;
		CqlProtocolByte flags_;
		CqlProtocolConsistency serialConsistency_;
		CqlProtocolLong defaultTimestamp_;
	};
}

