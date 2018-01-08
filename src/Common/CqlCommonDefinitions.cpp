#include <array>
#include <CqlDriver/Common/CqlCommonDefinitions.hpp>

namespace cql {
	/** Write the text description of consistency level to stream */
	std::ostream& operator<<(std::ostream& stream, CqlConsistencyLevel level) {
		static auto staticNames = [] {
			std::array<const char*, static_cast<std::size_t>(CqlConsistencyLevel::Max_)> names;
			names[static_cast<std::size_t>(CqlConsistencyLevel::Any)] = "Any";
			names[static_cast<std::size_t>(CqlConsistencyLevel::One)] = "One";
			names[static_cast<std::size_t>(CqlConsistencyLevel::Two)] = "Two";
			names[static_cast<std::size_t>(CqlConsistencyLevel::Three)] = "Three";
			names[static_cast<std::size_t>(CqlConsistencyLevel::Quorum)] = "Quorum";
			names[static_cast<std::size_t>(CqlConsistencyLevel::All)] = "All";
			names[static_cast<std::size_t>(CqlConsistencyLevel::LocalQuorum)] = "LocalQuorum";
			names[static_cast<std::size_t>(CqlConsistencyLevel::EachQuorum)] = "EachQuorum";
			names[static_cast<std::size_t>(CqlConsistencyLevel::Serial)] = "Serial";
			names[static_cast<std::size_t>(CqlConsistencyLevel::LocalSerial)] = "LocalSerial";
			names[static_cast<std::size_t>(CqlConsistencyLevel::LocalOne)] = "LocalOne";
			return names;
		}();
		auto index = static_cast<std::size_t>(level);
		if (index >= staticNames.size() || staticNames[index] == nullptr) {
			stream << "Unknow";
		} else {
			stream << staticNames[index];
		}
		return stream;
	}
}

