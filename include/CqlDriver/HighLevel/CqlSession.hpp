#pragma once

namespace cql {
	/** Interface use to perform database operations, it should be a light weight object */
	class CqlSession {
	public:
		/** Virtual destructor */
		virtual ~CqlSession() = default;
		// TODO:
		// query(...)
		// execute(...)
	};
}

