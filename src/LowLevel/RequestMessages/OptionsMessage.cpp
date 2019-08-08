#include "./OptionsMessage.hpp"

namespace cql {
	/** The storage of OptionsMessage */
	template <>
	thread_local ReusableStorageType<OptionsMessage>
		ReusableStorageInstance<OptionsMessage>;

	/** Get description of this message */
	std::string OptionsMessage::toString() const {
		return "OptionsMessage()";
	}

	/** Encode message body to binary data */
	void OptionsMessage::encodeBody(const ConnectionInfo&, std::string&) const {
		// The body of an OPTIONS message should be empty
	}
}

