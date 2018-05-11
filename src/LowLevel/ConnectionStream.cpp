#include <CQLDriver/Common/Exceptions/LogicException.hpp>
#include "./ConnectionStream.hpp"

namespace cql {
	/** Get the stream id */
	ConnectionStream::IdType ConnectionStream::getStreamId() const {
		if (!isValid_) {
			throw LogicException(CQL_CODEINFO, "can't get stream id from an invalid stream");
		}
		return streamId_;
	}

	/** Get whether this object holds a valid and in use stream id */
	bool ConnectionStream::isValid() const {
		return isValid_;
	}

	/** Release this stream */
	void ConnectionStream::close() {
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

	/** Constructor */
	ConnectionStream::ConnectionStream() :
		isValid_(false),
		streamId_(0),
		freeStreamIds_(nullptr) { }

	/** Constructor */
	ConnectionStream::ConnectionStream(
		const IdType& streamId,
		const seastar::lw_shared_ptr<std::vector<IdType>>& freeStreamIds) :
		isValid_(true),
		streamId_(streamId),
		freeStreamIds_(freeStreamIds) { }

	/** Move constructor */
	ConnectionStream::ConnectionStream(ConnectionStream&& stream) noexcept :
		isValid_(stream.isValid_),
		streamId_(stream.streamId_),
		freeStreamIds_(std::move(stream.freeStreamIds_)) {
		stream.isValid_ = false;
	}

	/** Move assignment */
	ConnectionStream& ConnectionStream::operator=(ConnectionStream&& stream) noexcept {
		if (&stream != this) {
			close();
			isValid_ = stream.isValid_;
			streamId_ = stream.streamId_;
			freeStreamIds_ = std::move(stream.freeStreamIds_);
			stream.isValid_ = false;
		}
		return *this;
	}

	/** Destructor */
	ConnectionStream::~ConnectionStream() {
		close();
	}
}

