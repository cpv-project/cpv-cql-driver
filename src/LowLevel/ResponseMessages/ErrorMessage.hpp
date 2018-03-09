#pragma once
#include <CQLDriver/Common/CommonDefinitions.hpp>
#include "../ProtocolTypes/ProtocolInt.hpp"
#include "../ProtocolTypes/ProtocolString.hpp"
#include "./ResponseMessageBase.hpp"

namespace cql {
	/** Indicates an error processing a request */
	class ErrorMessage : public ResponseMessageBase {
	public:
		using ResponseMessageBase::freeResources;

		/** For Object */
		void reset(MessageHeader&& header);

		/** Get description of this message */
		seastar::sstring toString() const override;

		/** Decode message body from binary data */
		void decodeBody(const ConnectionInfo& info, seastar::temporary_buffer<char>&& buffer) override;

		/** The error code, check native_protocol_v4.spec section 9 */
		ErrorCode getErrorCode() const { return static_cast<ErrorCode>(errorCode_.get()); }
		void setErrorCode(ErrorCode errorCode) { errorCode_.set(enumValue(errorCode)); }

		/** The error message */
		const ProtocolString& getErrorMessage() const& { return errorMessage_; }
		ProtocolString& getErrorMessage() & { return errorMessage_; }

		/** The extra contents, depends on error code */
		const seastar::sstring& getExtraContents() const& { return extraContents_; }
		seastar::sstring& getExtraContents() & { return extraContents_; }

		/** Constructor */
		ErrorMessage();

	private:
		ProtocolInt errorCode_;
		ProtocolString errorMessage_;
		seastar::sstring extraContents_;
	};
}

