#include "./QueryMessage.hpp"
#include "../ProtocolTypes/ProtocolInt.hpp"

namespace cql {
	/** The storage of QueryMessage */
	template <>
	thread_local ReusableStorageType<QueryMessage>
		ReusableStorageInstance<QueryMessage>;

	/** For Reusable */
	void QueryMessage::reset(MessageHeader&& header) {
		RequestMessageBase::reset(std::move(header));
		queryParameters_.reset();
	}

	/** Get description of this message */
	std::string QueryMessage::toString() const {
		std::string query;
		auto& command = queryParameters_.getCommand();
		if (command.isValid()) {
			query.append(command.getQuery().get());
		}
		return joinString("", "QueryMessage(query: ", query, ")");
	}

	/** Encode message body to binary data */
	void QueryMessage::encodeBody(const ConnectionInfo&, std::string& data) const {
		// The body of the message must be: <query><query_parameters>
		// where <query> is a [long string] represeting the query, and <query_parameters> must be:
		// (check comments on ProtocolQueryParameters)
		auto& command = queryParameters_.getCommand();
		if (!command.isValid()) {
			throw LogicException(CQL_CODEINFO, "invalid(moved) command");
		}
		// <query>
		auto query = command.getQuery().get();
		ProtocolInt querySize(query.size());
		querySize.encode(data);
		data.append(query);
		// <query_parameters>
		queryParameters_.encode(data);
	}

	/** Constructor */
	QueryMessage::QueryMessage() :
		queryParameters_() { }
}

