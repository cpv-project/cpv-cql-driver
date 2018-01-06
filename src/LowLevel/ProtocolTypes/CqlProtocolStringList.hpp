#pragma once
#include <cstdint>
#include <vector>
#include <core/sstring.hh>
#include "CqlProtocolString.hpp"

namespace cql {
	/**
	 * A [short] n, followed by n [string]
	 */
	class CqlProtocolStringList {
	public:
		using LengthType = std::uint16_t;

		const std::vector<CqlProtocolString>& get() const& { return values_; }
		std::vector<CqlProtocolString>& get() & { return values_; }

		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		CqlProtocolStringList() : values_() { }
		CqlProtocolStringList(std::vector<CqlProtocolString>&& values) :
			values_(std::move(values)) { }

	private:
		std::vector<CqlProtocolString> values_;
	};
}

