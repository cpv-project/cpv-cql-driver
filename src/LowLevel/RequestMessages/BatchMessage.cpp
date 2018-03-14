#include "./BatchMessage.hpp"
#include "../ProtocolTypes/ProtocolByte.hpp"
#include "../ProtocolTypes/ProtocolShort.hpp"
#include "../ProtocolTypes/ProtocolInt.hpp"

namespace cql {
	/** For Object */
	void BatchMessage::reset(MessageHeader&& header) {
		RequestMessageBase::reset(std::move(header));
		batchParameters_.reset();
		preparedQueryIds_.resize(0);
	}

	/** Get description of this message */
	std::string BatchMessage::toString() const {
		std::string str("BatchMessage(");
		auto& batchCommand = batchParameters_.getBatchCommand();
		if (batchCommand.isValid()) {
			str.append(joinString("", "type: ", batchCommand.getType(), ", queries: "));
			for (std::size_t i = 0, j = batchCommand.getQueryCount(); i < j; ++i) {
				auto query = batchCommand.getQuery(i);
				str.append(query.first, query.second);
				str.append("; ");
			}
		}
		str.append(")");
		return str;
	}

	/** Encode message body to binary data */
	void BatchMessage::encodeBody(const ConnectionInfo&, std::string& data) const {
		// The body of the message must be:
		// <type>
		// <query_list> which is:
		//   <n><query_1>...<query_n>
		// <batch_parameters> which is:
		//   <consistency><flags>[<serial_consistency>][<timestamp>]
		auto& batchCommand = batchParameters_.getBatchCommand();
		if (!batchCommand.isValid()) {
			throw LogicException(CQL_CODEINFO, "invalid(moved) command");
		}
		// <type>
		ProtocolByte type(enumValue(batchCommand.getType()));
		type.encode(data);
		// <n>, sum(parameter set count for query in queries)
		std::size_t queryCountValue = 0;
		for (std::size_t i = 0, j = batchCommand.getQueryCount(); i < j; ++i) {
			auto& parameterSets = batchCommand.getParameterSets(i);
			if (parameterSets.empty()) {
				// no parameter set means execute just once
				++queryCountValue;
			} else {
				queryCountValue += parameterSets.size();
			}
		}
		ProtocolShort queryCount(queryCountValue);
		if (queryCount.get() != queryCountValue) {
			throw LogicException(CQL_CODEINFO, "too many queries");
		}
		queryCount.encode(data);
		// <query_i>
		ProtocolByte kind;
		ProtocolInt stringSize; // query string size
		ProtocolShort idSize; // prepared query id size
		std::pair<const char*, std::size_t> stringOrId;
		ProtocolShort parameterCount;
		const auto& constSelf = *this;
		std::size_t queryCountVerify = 0;
		for (std::size_t i = 0, j = batchCommand.getQueryCount(); i < j; ++i) {
			auto& preparedQueryId = constSelf.getPreparedQueryId(i);
			if (preparedQueryId.empty()) {
				kind.set(enumValue(BatchQueryKind::Query));
				stringOrId = batchCommand.getQuery(i);
				stringSize.set(stringOrId.second);
			} else {
				kind.set(enumValue(BatchQueryKind::PreparedQueryId));
				stringOrId = { preparedQueryId.data(), preparedQueryId.size() };
				idSize.set(stringOrId.second);
			}
			auto& parameterSets = batchCommand.getParameterSets(i);
			if (parameterSets.empty()) {
				// no parameter set means execute just once
				// <kind>
				kind.encode(data);
				// <string_or_id>
				if (preparedQueryId.empty()) {
					stringSize.encode(data);
				} else {
					idSize.encode(data);
				}
				data.append(stringOrId.first, stringOrId.second);
				// <n>, parameters count
				parameterCount.set(0);
				parameterCount.encode(data);
				++queryCountVerify;
			}
			for (auto& parameterSet : parameterSets) {
				// <kind>
				kind.encode(data);
				// <string_or_id>
				if (preparedQueryId.empty()) {
					stringSize.encode(data);
				} else {
					idSize.encode(data);
				}
				data.append(stringOrId.first, stringOrId.second);
				// <n>, parameters count
				parameterCount.set(parameterSet.first);
				if (parameterCount.get() != parameterSet.first) {
					throw LogicException(CQL_CODEINFO, "too many parameters");
				}
				parameterCount.encode(data);
				// <value_1>...<value_n>
				data.append(parameterSet.second.data(), parameterSet.second.size());
				queryCountVerify += parameterSet.first;
			}
		}
		if (queryCountValue != queryCountVerify) {
			throw LogicException(CQL_CODEINFO, "incorrect query count calculation");
		}
		// <batch_parameters>
		batchParameters_.encode(data);
	}

	/** Get the prepared query id of the query at the specificed index */
	const std::string& BatchMessage::getPreparedQueryId(std::size_t index) const& {
		thread_local static std::string empty;
		if (index < preparedQueryIds_.size()) {
			return preparedQueryIds_[index];
		} else {
			return empty;
		}
	}

	/** Get the prepared query id of the query at the specificed index */
	std::string& BatchMessage::getPreparedQueryId(std::size_t index) & {
		if (index < preparedQueryIds_.size()) {
			return preparedQueryIds_[index];
		} else {
			auto& batchCommand = batchParameters_.getBatchCommand();
			if (batchCommand.isValid()) {
				preparedQueryIds_.resize(batchCommand.getQueryCount());
			}
			return preparedQueryIds_.at(index);
		}
	}

	/** Constructor */
	BatchMessage::BatchMessage() :
		batchParameters_(),
		preparedQueryIds_() { }
}

