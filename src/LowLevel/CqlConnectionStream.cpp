#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include "CqlConnectionStream.hpp"

namespace cql {
	/** Constructor */
	CqlConnectionStream::CqlConnectionStream(
		std::uint16_t streamId, const seastar::lw_shared_ptr<State>& state) :
		streamId_(streamId),
		state_(state) {
		if (state_.get() != nullptr) {
			if (state_->isInUse) {
				throw CqlLogicException(CQL_CODEINFO, "construct stream with a already in use state");
			}
			state_->isInUse = true;
		}
	}

	/** Move constructor */
	CqlConnectionStream::CqlConnectionStream(CqlConnectionStream&& stream) :
		streamId_(stream.streamId_),
		state_(std::move(stream.state_)) {
		stream.state_ = nullptr;
	}

	/** Move assignment */
	CqlConnectionStream& CqlConnectionStream::operator=(CqlConnectionStream&& stream) {
		if (&stream != this) {
			streamId_ = stream.streamId_;
			state_ = std::move(stream.state_);
			stream.state_ = nullptr;
		}
		return *this;
	}

	/** Destructor */
	CqlConnectionStream::~CqlConnectionStream() {
		if (state_.get() != nullptr) {
			// no longer use this stream
			state_->isInUse = false;
		}
	}
}

