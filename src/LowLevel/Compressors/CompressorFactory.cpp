#include "./CompressorFactory.hpp"
#include "./LZ4Compressor.hpp"

namespace cql {
	namespace CompressorFactory {
		/** Get a suitable compressor supported by both client and server */
		seastar::shared_ptr<CompressorBase> getCompressor(
			const ProtocolStringMultiMap& options) {
			// TODO
			return nullptr;
		}
	}
}


