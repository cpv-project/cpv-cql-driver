#include "CqlStartupMessage.hpp"

namespace cql {
	namespace {
		CqlProtocolString CqlVersionKey("CQL_VERSION");
		CqlProtocolString CompressionKey("COMPRESSION");
		CqlProtocolString CqlVersionValue("3.0.0");
	}

	/** For CqlObject */
	void CqlStartupMessage::reset(CqlMessageHeader&& header) {
		CqlRequestMessageBase::reset(std::move(header));
		options_.reset();
		options_.get().emplace(CqlVersionKey, CqlVersionValue);
	}

	/** Encode message body to binary data */
	void CqlStartupMessage::encodeBody(const CqlConnectionInfo&, seastar::sstring& data) const {
		// The body is a [string map] of options, possible options are
		// - "CQL_VERSION", the version of CQL to use, the only version supported is "3.0.0"
		// - "COMPRESSION", the compression algorithm to use for frames, this is optional
		options_.encode(data);
	}

	/** Set the compression algorithm to use, support "lz4", "snappy", or nullptr */
	void CqlStartupMessage::setCompression(const char* algorithm) {
		if (algorithm != nullptr) {
			options_.get().emplace(CompressionKey, CqlProtocolString(algorithm));
		} else {
			options_.get().erase(CompressionKey);
		}
	}

	/** Constructor */
	CqlStartupMessage::CqlStartupMessage() :
		options_() { }
}

