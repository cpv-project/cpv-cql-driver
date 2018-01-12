#include <CqlDriver/Common/Exceptions/CqlNotImplementedException.hpp>
#include "CqlConnectionInfo.hpp"

namespace cql {
	/** Choose a version for this connection */
	void CqlConnectionInfo::useVersion(std::size_t version) {
		if (version == 4) {
			version_ = version;
			headerSize_ = 9; // version 1 + flags 1 + stream 2 + opcode 1 + length 4
			maximumMessageBodySize_ = 256 * 1024 * 1024; // 256MB
		} else {
			throw CqlNotImplementedException(CQL_CODEINFO, "for now only version 4 is implemented");
		}
	}

	/** Constructor */
	CqlConnectionInfo::CqlConnectionInfo() :
		version_(),
		headerSize_(),
		maximumMessageBodySize_() {
		useVersion(4);
	}
}

