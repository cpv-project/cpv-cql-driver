#include "./CompressorFactory.hpp"
#include "./LZ4Compressor.hpp"

namespace cql {
	namespace CompressorFactory {
		namespace {
			ProtocolString CompressionKey("COMPRESSION");
		}

		/** Get a suitable compressor supported by both client and server */
		seastar::shared_ptr<CompressorBase> getCompressor(
			const ProtocolStringMultiMap& options) {
			auto algorithmsIt = options.get().find(CompressionKey);
			if (algorithmsIt == options.get().end()) {
				return nullptr;
			}
			for (const auto& algorithm : algorithmsIt->second.get()) {
				if (algorithm.get() == "lz4") {
					thread_local static seastar::shared_ptr<CompressorBase> compressor = (
						seastar::make_shared<LZ4Compressor>());
					return compressor;
				}
			}
			return nullptr;
		}
	}
}

