#pragma once
#include "CqlConnectionStream.hpp"

namespace cql {
	class CqlConnection;

	/** Class used to create and reuse database connections */
	class CqlConnectionPool :
		public seastar::enable_lw_shared_from_this<CqlConnection> {
	public:
		

	private:
		std::vector<seastar::lw_shared_ptr<CqlConnection>> connections_;
	};
}

