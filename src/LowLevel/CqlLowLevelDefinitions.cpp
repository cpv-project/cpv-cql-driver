#include "CqlLowLevelDefinitions.hpp"
#include <array>
#include <vector>
#include <utility>

namespace cql {
	/** Write the text description of message type to stream */
	std::ostream& operator<<(std::ostream& stream, CqlMessageType type) {
		static auto staticNames = [] {
			std::array<const char*, static_cast<std::size_t>(CqlMessageType::Max_)> names;
			names[static_cast<std::size_t>(CqlMessageType::Error)] = "Error";
			names[static_cast<std::size_t>(CqlMessageType::StartUp)] = "StartUp";
			names[static_cast<std::size_t>(CqlMessageType::Ready)] = "Ready";
			names[static_cast<std::size_t>(CqlMessageType::Authenticate)] = "Authenticate";
			names[static_cast<std::size_t>(CqlMessageType::Options)] = "Options";
			names[static_cast<std::size_t>(CqlMessageType::Supported)] = "Supported";
			names[static_cast<std::size_t>(CqlMessageType::Query)] = "Query";
			names[static_cast<std::size_t>(CqlMessageType::Result)] = "Result";
			names[static_cast<std::size_t>(CqlMessageType::Prepare)] = "Prepare";
			names[static_cast<std::size_t>(CqlMessageType::Execute)] = "Execute";
			names[static_cast<std::size_t>(CqlMessageType::Register)] = "Register";
			names[static_cast<std::size_t>(CqlMessageType::Event)] = "Event";
			names[static_cast<std::size_t>(CqlMessageType::Batch)] = "Batch";
			names[static_cast<std::size_t>(CqlMessageType::AuthChallenge)] = "AuthChallenge";
			names[static_cast<std::size_t>(CqlMessageType::AuthResponse)] = "AuthResponse";
			names[static_cast<std::size_t>(CqlMessageType::AuthSuccess)] = "AuthSuccess";
			return names;
		}();
		auto index = static_cast<std::size_t>(type);
		if (index >= staticNames.size() || staticNames[index] == nullptr) {
			stream << "Unknow";
		} else {
			stream << staticNames[index];
		}
		return stream;
	}

	/** Or operation */
	CqlMessageHeaderFlags operator|(CqlMessageHeaderFlags a, CqlMessageHeaderFlags b) {
		return CqlMessageHeaderFlags(static_cast<std::size_t>(a) | static_cast<std::size_t>(b));
	}

	/** And operation */
	CqlMessageHeaderFlags operator&(CqlMessageHeaderFlags a, CqlMessageHeaderFlags b) {
		return CqlMessageHeaderFlags(static_cast<std::size_t>(a) & static_cast<std::size_t>(b));
	}

	/** Write the text description of header flags to stream */
	std::ostream& operator<<(std::ostream& stream, CqlMessageHeaderFlags flags) {
		static std::vector<std::pair<CqlMessageHeaderFlags, const char*>> staticNames({
			{ CqlMessageHeaderFlags::Compression, "Compression" },
			{ CqlMessageHeaderFlags::Tracing, "Tracing" },
			{ CqlMessageHeaderFlags::CustomPayload, "CustomPayload" },
			{ CqlMessageHeaderFlags::Warning, "Warning" },
			{ CqlMessageHeaderFlags::UseBeta, "UseBeta" }
		});
		bool isFirst = true;
		for (const auto& name : staticNames) {
			if ((flags & name.first) == name.first) {
				if (isFirst) {
					isFirst = false;
				} else {
					stream << "|";
				}
				stream << name.second;
			}
		}
		return stream;
	}
}

