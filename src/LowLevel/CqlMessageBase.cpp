#include "CqlMessageBase.hpp"

namespace cql {
	/** Get the message header */
	const CqlMessageHeader& CqlMessageBase::getHeader() const& {
		return header_;
	}

	/** Get the message header */
	CqlMessageHeader& CqlMessageBase::getHeader() & {
		return header_;
	}

	/** For CqlObject support */
	void CqlMessageBase::freeResources() { }

	/** For CqlObject support */
	void CqlMessageBase::reset(CqlMessageHeader&& header) {
		header_ = std::move(header);
	}

	/** Constructor */
	CqlMessageBase::CqlMessageBase() : header_() { }
}

