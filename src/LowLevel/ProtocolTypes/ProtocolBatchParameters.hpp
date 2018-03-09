#pragma once
#include <CQLDriver/Common/CommonDefinitions.hpp>
#include <CQLDriver/Common/BatchCommand.hpp>
#include "../LowLevelDefinitions.hpp"
#include "./ProtocolByte.hpp"

namespace cql {
	/**
	 * <consistency><flags>[<serial_consistency>][<timestamp>]
	 * [] mean optional, depends on <flags>.
	 * Check native_protocol_v4.spec section 4.1.7.
	 */
	class ProtocolBatchParameters {
	public:
		/** Reset to initial state */
		void reset();

		/** The batch command contains queries and parameters */
		void setBatchCommand(BatchCommand&& batchCommand);
		const BatchCommand& getBatchCommand() const& { return batchCommand_; }

		/** Get the flags that indicate which component is included */
		BatchParametersFlags getFlags() const {
			return static_cast<BatchParametersFlags>(flags_.get());
		}

		/** Encode and decode functions */
		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		/** Constructor */
		ProtocolBatchParameters();

	private:
		ProtocolByte flags_;
		BatchCommand batchCommand_;
	};
}

