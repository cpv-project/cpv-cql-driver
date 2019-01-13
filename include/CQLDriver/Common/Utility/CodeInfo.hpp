#pragma once
#include <iostream>
#include "./StringUtils.hpp"
#include "./Macros.hpp"

namespace cql {
	/** Code information wrapper class, see macro CQL_CODEINFO */
	class CodeInfo {
	public:
		/** Get the string inside this class */
		std::string str() const& { return str_; }
		std::string str() && { return std::move(str_); }

		/** Constructor */
		explicit CodeInfo(std::string&& str) :
			str_(std::move(str)) { }

	private:
		std::string str_;
	};
}

