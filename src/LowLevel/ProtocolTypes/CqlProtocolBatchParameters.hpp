#pragma once
#include <CqlDriver/Common/CqlCommonDefinitions.hpp>
#include <CqlDriver/Common/CqlBatchCommand.hpp>
#include "../CqlLowLevelDefinitions.hpp"
#include "CqlProtocolByte.hpp"

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

		/** The batch command contains queries and parameters */
		void setBatchCommand(CqlBatchCommand&& batchCommand);
		const CqlBatchCommand& getBatchCommand() const& { return batchCommand_; }

		/** Get the flags that indicate which component is included */
		CqlBatchParametersFlags getFlags() const {
			return static_cast<CqlBatchParametersFlags>(flags_.get());
		}

		/** Encode and decode functions */
		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		/** Constructor */
		CqlProtocolBatchParameters();

	private:
		CqlProtocolByte flags_;
		CqlBatchCommand batchCommand_;
	};
}

