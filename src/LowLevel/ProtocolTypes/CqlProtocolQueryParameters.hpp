#pragma once
#include <CqlDriver/Common/CqlCommonDefinitions.hpp>
#include <CqlDriver/Common/CqlCommand.hpp>
#include "../CqlLowLevelDefinitions.hpp"
#include "CqlProtocolByte.hpp"

namespace cql {
	/**
	 * <consistency><flags>[<n>[name_1]<value_1>...[name_n]<value_n>][<result_page_size>][<paging_state>] ~
	 * ~ [<serial_consistency>][<timestamp>]
	 * [] mean optional, depends on <flags>.
	 * Check native_protocol_v4.spec section 4.1.4.
	 */
	class CqlProtocolQueryParameters {
	public:
		/** Reset to initial state */
		void reset();

		/* Set whether to not receive metadata in result */
		void setSkipMetadata(bool value);

		/** The command contains query and parameters */
		void setCommand(CqlCommand&& command);
		const CqlCommand& getCommand() const& { return command_; }

		/** Get the flags that indicate which component is included */
		CqlQueryParametersFlags getFlags() const {
			return static_cast<CqlQueryParametersFlags>(flags_.get());
		}

		/** Encode and decode functions */
		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		/** Constructor */
		CqlProtocolQueryParameters();

	private:
		CqlProtocolByte flags_;
		CqlCommand command_;
	};
}

