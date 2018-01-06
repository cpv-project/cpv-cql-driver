#pragma once
#include <cstdint>
#include <core/sstring.hh>
#include "CqlProtocolString.hpp"

namespace cql {
	/**
	 * A [short] n, followed by n [string]
	 */
	class CqlProtocolStringList {
	public:
		using LengthType = std::uint16_t;

	private:
		std::vector<CqlProtocolString> values_;
	};
}

