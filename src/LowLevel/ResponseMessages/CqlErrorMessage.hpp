#pragma once
#include <CqlDriver/Common/CqlCommonDefinitions.hpp>
#include "../ProtocolTypes/CqlProtocolInt.hpp"
#include "../ProtocolTypes/CqlProtocolString.hpp"
#include "CqlResponseMessageBase.hpp"

namespace cql {
	/** Indicates an error processing a request */
	class CqlErrorMessage : public CqlResponseMessageBase {
	public:
		using CqlResponseMessageBase::freeResources;

		/** For CqlObject */
		void reset(CqlMessageHeader&& header);

		/** Get description of this message */
		seastar::sstring toString() const override;

		/** Decode message body from binary data */
		void decodeBody(const CqlConnectionInfo& info, const char*& ptr, const char* end) override;

		/** The error code, check native_protocol_v4.spec section 9 */
		CqlErrorCode getErrorCode() const { return static_cast<CqlErrorCode>(errorCode_.get()); }
		void setErrorCode(CqlErrorCode errorCode) { errorCode_.set(enumValue(errorCode)); }

		/** The error message */
		const CqlProtocolString& getErrorMessage() const& { return errorMessage_; }
		CqlProtocolString& getErrorMessage() & { return errorMessage_; }

		/** The extra contents, depends on error code */
		const seastar::sstring& getExtraContents() const& { return extraContents_; }
		seastar::sstring& getExtraContents() & { return extraContents_; }

		/** Constructor */
		CqlErrorMessage();

	private:
		CqlProtocolInt errorCode_;
		CqlProtocolString errorMessage_;
		seastar::sstring extraContents_;
	};
}

