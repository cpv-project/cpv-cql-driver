#pragma once
#include <seastar/core/future.hh>
#include "../Common/Utility/Reusable.hpp"
#include "../Common/Command.hpp"
#include "../Common/BatchCommand.hpp"
#include "../Common/ResultSet.hpp"

namespace cql {
	/** Defines members of Session */
	class SessionData;

	/** High level object that use to perform database operations */
	class Session {
	public:
		/** Check whether this is a valid session (will be false if moved) */
		bool isValid() const;

		/** Execute the query command and return the result set */
		seastar::future<ResultSet> query(Command&& command);

		/** Execute a single modification command */
		seastar::future<> execute(Command&& command);

		/** Perform multiple modification commands in batch */
		seastar::future<> batchExecute(BatchCommand&& command);

		/** Constructor */
		explicit Session(Reusable<SessionData>&& data);

	private:
		Reusable<SessionData> data_;
	};
}

