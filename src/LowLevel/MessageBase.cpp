#include "./MessageBase.hpp"

namespace cql {
	/** Get the message header */
	const MessageHeader& MessageBase::getHeader() const& {
		return header_;
	}

	/** Get the message header */
	MessageHeader& MessageBase::getHeader() & {
		return header_;
	}

	/** For Object support */
	void MessageBase::freeResources() { }

	/** For Object support */
	void MessageBase::reset(MessageHeader&& header) {
		header_ = std::move(header);
	}

	/** Constructor */
	MessageBase::MessageBase() : header_() { }
}

