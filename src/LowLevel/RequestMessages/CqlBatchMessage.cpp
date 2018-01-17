#include "CqlBatchMessage.hpp"

namespace cql {
	/** For CqlObject */
	void CqlBatchMessage::reset(CqlMessageHeader&& header) {
		CqlRequestMessageBase::reset(std::move(header));
		type_.reset();
		queries_.reset();
		batchParameters_.reset();
	}

	/** Encode message body to binary data */
	void CqlBatchMessage::encodeBody(const CqlConnectionInfo&, seastar::sstring& data) const {
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

