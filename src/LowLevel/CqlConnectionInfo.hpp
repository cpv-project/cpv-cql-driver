#pragma once
#include <cstdint>

namespace cql {
	/**
	 * Per connection information
	 * Affects how message is encoded and decoded
	 */
	class CqlConnectionInfo {
	public:
		/** The binary protocol version this connection using */
		std::size_t getVersion() const { return version_; }

		/** The header size of this binary protocol version */
		std::size_t getHeaderSize() const { return headerSize_; }

		/** The maximum message body size for security check (avoid oom attack) */
		std::size_t getMaximumMessageBodySize() const { return maximumMessageBodySize_; }

		/** Choose a version for this connection */
		void useVersion(std::size_t version);

		/** Constructor */
		CqlConnectionInfo();

	private:
		std::size_t version_;
		std::size_t headerSize_;
		std::size_t maximumMessageBodySize_;
	};
}

