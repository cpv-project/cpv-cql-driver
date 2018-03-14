#pragma once
#include <functional>
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
		void setBatchCommandRef(BatchCommand& batchCommand);
		const BatchCommand& getBatchCommand() const& { return batchCommandRef_.get(); }

		/** Get the flags that indicate which component is included */
		BatchParametersFlags getFlags() const {
			return static_cast<BatchParametersFlags>(flags_.get());
		}

		/** Encode and decode functions */
		void encode(std::string& data) const;
		void decode(const char*& ptr, const char* end);

		/** Constructor */
		ProtocolBatchParameters();

		/** Move constructor */
		ProtocolBatchParameters(ProtocolBatchParameters&& other);

		/** Move assignment */
		ProtocolBatchParameters& operator=(ProtocolBatchParameters&& other);

		/** Disallow copy */
		ProtocolBatchParameters(const ProtocolBatchParameters&) = delete;
		ProtocolBatchParameters& operator=(const ProtocolBatchParameters&) = delete;

	private:
		ProtocolByte flags_;
		BatchCommand batchCommand_;
		std::reference_wrapper<BatchCommand> batchCommandRef_;
	};
}

