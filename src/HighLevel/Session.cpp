#include <CQLDriver/HighLevel/Session.hpp>
#include <CQLDriver/Common/Exceptions/NotImplementedException.hpp>

namespace cql {
	/** Execute the query command and return the result set */
	seastar::future<ResultSet> Session::query(Command&& command) {
		throw NotImplementedException(CQL_CODEINFO);
	}

	/** Execute a single modification command */
	seastar::future<> Session::execute(Command&& command) {
		throw NotImplementedException(CQL_CODEINFO);
	}

	/** Perform multiple modification commands in batch */
	seastar::future<> Session::batchExecute(BatchCommand command) {
		throw NotImplementedException(CQL_CODEINFO);
	}

	/** Constructor */
	Session::Session(Object<SessionData>&& data) :
		data_(std::move(data)) { }
}

