#pragma once
#include <CQLDriver/Common/CommonDefinitions.hpp>
#include <CQLDriver/Common/Command.hpp>
#include "../LowLevelDefinitions.hpp"
#include "./ProtocolByte.hpp"

namespace cql {
	/**
	 * <consistency><flags>[<n>[name_1]<value_1>...[name_n]<value_n>][<result_page_size>][<paging_state>] ~
	 * ~ [<serial_consistency>][<timestamp>]
	 * [] mean optional, depends on <flags>.
	 * Check native_protocol_v4.spec section 4.1.4.
	 */
	class ProtocolQueryParameters {
	public:
		/** Reset to initial state */
		void reset();

		/* Set whether to not receive metadata in result */
		void setSkipMetadata(bool value);

		/** The command contains query and parameters */
		void setCommand(Command&& command);
		const Command& getCommand() const& { return command_; }

		/** Get the flags that indicate which component is included */
		QueryParametersFlags getFlags() const {
			return static_cast<QueryParametersFlags>(flags_.get());
		}

		/** Encode and decode functions */
		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		/** Constructor */
		ProtocolQueryParameters();

	private:
		ProtocolByte flags_;
		Command command_;
	};
}

