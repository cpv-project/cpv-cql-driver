#include "CqlBatchMessage.hpp"

namespace cql {
	/** For CqlObject */
	void CqlBatchMessage::reset(CqlMessageHeader&& header) {
		CqlRequestMessageBase::reset(std::move(header));
		type_.reset();
		queries_.reset();
		batchParameters_.reset();
	}

	/** Get description of this message */
	seastar::sstring CqlBatchMessage::toString() const {
		return joinString("", "CqlBatchMessage(type: ", getType(), ")");
	}

	/** Encode message body to binary data */
	void CqlBatchMessage::encodeBody(const CqlConnectionInfo&, seastar::sstring& data) const {
		// The body of the message must be:
		// <type>
		// <query_list> which is:
		//   <n><query_1>...<query_n>
		// <batch_parameters> which is:
		//   <consistency><flags>[<serial_consistency>][<timestamp>]
		type_.encode(data);
		queries_.encode(data);
		batchParameters_.encode(data);
	}

	/** Get the type of batch */
	CqlBatchType CqlBatchMessage::getType() const {
		return static_cast<CqlBatchType>(type_.get());
	}

	/** Set the type of batch */
	void CqlBatchMessage::setType(CqlBatchType type) {
		type_.set(enumValue(type));
	}

	/** Constructor */
	CqlBatchMessage::CqlBatchMessage() :
		type_(),
		queries_(),
		batchParameters_() { }
}

