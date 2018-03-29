#pragma once
#include <core/shared_ptr.hh>
#include "../ProtocolTypes/ProtocolStringMultiMap.hpp"
#include "./CompressorBase.hpp"

namespace cql {
	namespace CompressorFactory {
		/**
		 * Get a suitable compressor supported by both client and server.
		 * If there is no such compressor, this function will return nullptr.
		 */
		seastar::shared_ptr<CompressorBase> getCompressor(
			const ProtocolStringMultiMap& options);
	}
}

