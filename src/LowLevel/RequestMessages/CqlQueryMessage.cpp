#include "CqlQueryMessage.hpp"
#include "../ProtocolTypes/CqlProtocolInt.hpp"

namespace cql {
	/** For CqlObject */
	void CqlQueryMessage::reset(CqlMessageHeader&& header) {
		CqlRequestMessageBase::reset(std::move(header));
		queryParameters_.reset();
	}

	/** Get description of this message */
	seastar::sstring CqlQueryMessage::toString() const {
		seastar::sstring query;
		auto& command = queryParameters_.getCommand();
		if (command.isValid()) {
			query.append(command.getQuery().first, command.getQuery().second);
		}
		return joinString("", "CqlQueryMessage(query: ", query, ")");
	}

	/** Encode message body to binary data */
	void CqlQueryMessage::encodeBody(const CqlConnectionInfo&, seastar::sstring& data) const {
		// The body of the message must be: <query><query_parameters>
		// where <query> is a [long string] represeting the query, and <query_parameters> must be:
		// (check comments on CqlProtocolQueryParameters)
		auto& command = queryParameters_.getCommand();
		if (!command.isValid()) {
			throw CqlLogicException(CQL_CODEINFO, "invalid(moved) command");
		}
		// <query>
		auto query = command.getQuery();
		CqlProtocolInt querySize(query.second);
		querySize.encode(data);
		data.append(query.first, query.second);
		// <query_parameters>
		queryParameters_.encode(data);
	}

	/** Constructor */
	CqlQueryMessage::CqlQueryMessage() :
		queryParameters_() { }
}

