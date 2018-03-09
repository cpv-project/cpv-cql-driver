#include "./StartupMessage.hpp"

namespace cql {
	namespace {
		ProtocolString VersionKey("CQL_VERSION");
		ProtocolString CompressionKey("COMPRESSION");
		ProtocolString VersionValue("3.0.0");
	}

	/** For Object */
	void StartupMessage::reset(MessageHeader&& header) {
		RequestMessageBase::reset(std::move(header));
		options_.reset();
		options_.get().emplace(VersionKey, VersionValue);
	}

	/** Get description of this message */
	seastar::sstring StartupMessage::toString() const {
		return "StartupMessage()";
	}

	/** Encode message body to binary data */
	void StartupMessage::encodeBody(const ConnectionInfo&, seastar::sstring& data) const {
		// The body is a [string map] of options, possible options are
		// - "CQL_VERSION", the version of CQL to use, the only version supported is "3.0.0"
		// - "COMPRESSION", the compression algorithm to use for frames, this is optional
		options_.encode(data);
	}

	/** Set the compression algorithm to use, support "lz4", "snappy", or nullptr */
	void StartupMessage::setCompression(const char* algorithm) {
		if (algorithm != nullptr) {
			options_.get().emplace(CompressionKey, ProtocolString(algorithm));
		} else {
			options_.get().erase(CompressionKey);
		}
	}

	/** Constructor */
	StartupMessage::StartupMessage() :
		options_() { }
}

