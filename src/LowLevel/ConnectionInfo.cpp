#include <CQLDriver/Common/Exceptions/NotImplementedException.hpp>
#include <CQLDriver/Common/CommonDefinitions.hpp>
#include "./ConnectionInfo.hpp"

namespace cql {
	/** Choose a version for this connection */
	void ConnectionInfo::useVersion(std::size_t version) {
		if (CQL_LIKELY(version == 4)) {
			version_ = version;
			headerSize_ = 9; // version 1 + flags 1 + stream 2 + opcode 1 + length 4
			maximumMessageBodySize_ = 256 * 1024 * 1024; // 256MB
		} else {
			throw NotImplementedException(CQL_CODEINFO, "for now only version 4 is implemented");
		}
	}

	/** Constructor */
	ConnectionInfo::ConnectionInfo() :
		version_(),
		headerSize_(),
		maximumMessageBodySize_() {
		useVersion(4);
	}
}

