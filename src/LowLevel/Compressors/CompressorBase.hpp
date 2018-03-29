#pragma once
#include <string>
#include <core/temporary_buffer.hh>

namespace cql {
	/* The interface used to compress a request frame or decompress a response frame */
	class CompressorBase {
	public:
		/** Get the name of the compressor */
		virtual const std::string& name() const& = 0;

		/** Compress the request frame's body */
		virtual void compress(const std::string& source, std::string& output) = 0;

		/** Decompress the response frame's body */
		virtual seastar::temporary_buffer<char> decompress(seastar::temporary_buffer<char>&& source) = 0;
	};
}

