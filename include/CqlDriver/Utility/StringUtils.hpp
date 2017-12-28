#pragma once
#include <vector>
#include <memory>
#include <sstream>

namespace cql {
	/**
	 * Split string with specified characters.
	 * Call func(startIndex, endIndex, count) while split.
	 * Default split characters are empty characters.
	 */
	template <class Func>
	static void splitString(
		const std::string& str, const Func& func, const char* chars = " \t\r\n") {
		std::size_t startIndex = 0;
		std::size_t count = 0;
		while (startIndex < str.size()) {
			auto index = str.find_first_of(chars, startIndex);
			auto endIndex = (index == str.npos) ? str.size() : index;
			func(startIndex, endIndex, count);
			index = str.find_first_not_of(chars, endIndex);
			startIndex = (index == str.npos) ? str.size() : index;
			++count;
		}
	}

	/**
	 * Join arguments into string.
	 * This function is very slow, don't call it where performance is important.
	 */
	template <class Delimiter, class... Args>
	static std::string joinString(Delimiter&& delimiter, Args&&... args) {
		std::ostringstream stream;
		bool isFirst = true;
		auto func = [&stream, &isFirst, &delimiter](auto&& arg) {
			if (isFirst) {
				isFirst = false;
			} else {
				stream << delimiter;
			}
			stream << arg;
			return 0;
		};
		int dummy[sizeof...(Args)] = { func(std::forward<Args>(args))... };
		return stream.str();
	}
}

