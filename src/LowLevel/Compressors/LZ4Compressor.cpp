#include <lz4.h>
#include <cstdint>
#include <cstring>
#include <core/byteorder.hh>
#include <CQLDriver/Common/Exceptions/LogicException.hpp>
#include "LZ4Compressor.hpp"

namespace cql {
	/**
	 * About the endian of uncompressed length:
	 * It's big endian in both scylla and cassandra,
	 * the specification didn't tell but the code used to compress frame indicates that.
	 * See "transport/server.cc" in scylla,
	 * and "transport/FrameCompressor.java" in cassandra.
	 */

	/** Get the name of the compressor */
	const std::string& LZ4Compressor::name() const& {
		thread_local static std::string nameVal = "lz4";
		return nameVal;
	}

	/** Compress the request frame's body */
	void LZ4Compressor::compress(const std::string& source, std::string& output) {
		std::uint32_t sourceSize = source.size();
		std::size_t outputPreviousSize = output.size();
		std::size_t outputAdditionSize = sizeof(sourceSize) + LZ4_compressBound(source.size());
		output.resize(outputPreviousSize + outputAdditionSize);
		auto ret = LZ4_compress_default(
			source.data(),
			&output[outputPreviousSize + sizeof(sourceSize)],
			sourceSize,
			outputAdditionSize - sizeof(sourceSize));
		if (ret <= 0) {
			throw LogicException(CQL_CODEINFO, "lz4 compress failed, ret:", ret);
		}
		output.resize(outputPreviousSize + sizeof(sourceSize) + ret);
		sourceSize = seastar::cpu_to_be(sourceSize);
		std::memcpy(&output[outputPreviousSize], &sourceSize, sizeof(sourceSize));
	}

	/** Decompress the response frame's body */
	seastar::temporary_buffer<char> LZ4Compressor::decompress(
		seastar::temporary_buffer<char>&& source) {
		std::uint32_t outputSize = 0;
		if (source.size() < sizeof(outputSize)) {
			return seastar::temporary_buffer<char>();
		}
		std::memcpy(&outputSize, source.get(), sizeof(outputSize));
		std::cout << +source.get()[0] << std::endl;
		std::cout << +source.get()[1] << std::endl;
		std::cout << +source.get()[2] << std::endl;
		std::cout << +source.get()[3] << std::endl;
		outputSize = seastar::be_to_cpu(outputSize);
		seastar::temporary_buffer<char> output(outputSize);
		auto ret = LZ4_decompress_safe(
			source.get() + sizeof(outputSize),
			output.get_write(),
			source.size() - sizeof(outputSize),
			output.size());
		if (ret != outputSize) {
			throw LogicException(CQL_CODEINFO, "lz4 decompress failed, ret:", ret, "!=", outputSize);
		}
		return output;
	}
}

