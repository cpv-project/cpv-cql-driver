#pragma once

namespace cql {
	/** Interface use to perform database operations, it should be a light weight object */
	class Session {
	public:
		/** Virtual destructor */
		virtual ~Session() = default;
		// TODO:
		// query(...)
		// execute(...)
	};
}

