#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include "CqlConnectionStream.hpp"

namespace cql {
	/** Get the stream id */
	CqlConnectionStream::IdType CqlConnectionStream::getStreamId() const {
		if (!isValid_) {
			throw CqlLogicException(CQL_CODEINFO, "can't get stream id from invalid stream");
		}
		return streamId_;
	}

	/** Get whether this object hold a valid and in use stream id */
	bool CqlConnectionStream::isValid() const {
		return isValid_;
	}

	/** Constructor */
	CqlConnectionStream::CqlConnectionStream() :
		isValid_(false),
		streamId_(0),
		freeStreamIds_(nullptr) { }

	/** Constructor */
	CqlConnectionStream::CqlConnectionStream(
		IdType streamId,
		const seastar::lw_shared_ptr<std::vector<IdType>>& freeStreamIds) :
		isValid_(true),
		streamId_(streamId),
		freeStreamIds_(freeStreamIds) { }

	/** Move constructor */
	CqlConnectionStream::CqlConnectionStream(CqlConnectionStream&& stream) :
		isValid_(stream.isValid_),
		streamId_(stream.streamId_),
		freeStreamIds_(std::move(stream.freeStreamIds_)) {
		stream.isValid_ = false;
	}

	/** Move assignment */
	CqlConnectionStream& CqlConnectionStream::operator=(CqlConnectionStream&& stream) {
		if (&stream != this) {
			isValid_ = stream.isValid_;
			streamId_ = stream.streamId_;
			freeStreamIds_ = std::move(stream.freeStreamIds_);
			stream.isValid_ = false;
		}
		return *this;
	}

	/** Destructor */
	CqlConnectionStream::~CqlConnectionStream() {
		if (isValid_) {
			isValid_ = false;
			if (freeStreamIds_.get() != nullptr) {
				freeStreamIds_->emplace_back(streamId_);
			} else {
				// throw an exception will abort the program, just output the error log
				std::cerr << "valid stream can't hold a null stream ids" << std::endl;
			}
		}
	}
}

