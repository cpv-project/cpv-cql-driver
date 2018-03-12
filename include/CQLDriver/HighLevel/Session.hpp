#pragma once
#include <core/future.hh>
#include "../Common/Utility/Object.hpp"
#include "../Common/Command.hpp"
#include "../Common/BatchCommand.hpp"

namespace cql {
	/** Defines members of Session */
	class SessionData;

	/**
	 * TODO: fill this class, move this class to other file
	 */
	class ResultSet { };

	/** High level object that use to perform database operations */
	class Session {
	public:
		/** Execute the query command and return the result set */
		seastar::future<ResultSet> query(Command&& command);

		/** Execute a single modification command */
		seastar::future<> execute(Command&& command);

		/** Perform multiple modification commands in batch */
		seastar::future<> batchExecute(BatchCommand command);

		/** Constructor */
		explicit Session(Object<SessionData>&& data);

	private:
		Object<SessionData> data_;
	};
}

