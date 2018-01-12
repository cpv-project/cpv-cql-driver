#include "CqlMessageBase.hpp"

namespace cql {
	/** Get the message header */
	const CqlMessageHeader& CqlMessageBase::getHeader() const& {
		return header_;
	}

	/** Constructor */
	CqlMessageBase::CqlMessageBase(CqlMessageHeader&& header) :
		header_(std::move(header)) { }
}

