#pragma once
#include "CompressorBase.hpp"

namespace cql {
	/**
	 * Compressor uses lz4 algorithm.
	 * The first four bytes of the body will be the uncompressed length (little endian),
	 * followed by the compressed bytes.
	 */
	class LZ4Compressor : public CompressorBase {
	public:
		/** Get the name of the compressor */
		virtual const std::string& name() const& override;

		/** Compress the request frame's body */
		virtual void compress(const std::string& source, std::string& output) override;

		/** Decompress the response frame's body */
		virtual seastar::temporary_buffer<char> decompress(
			seastar::temporary_buffer<char>&& source) override;	
	};
}

