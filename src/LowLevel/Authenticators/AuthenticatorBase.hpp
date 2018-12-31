#pragma once
#include <seastar/core/future.hh>
#include <seastar/core/shared_ptr.hh>

namespace cql {
	class Connection;
	class ConnectionStream;

	/** The interface used to perform the authentication on cql database connection */
	class AuthenticatorBase {
	public:
		/** Perform the authentication */
		virtual seastar::future<> authenticate(
			const seastar::lw_shared_ptr<Connection>& connection,
			const ConnectionStream& stream) const = 0;

		/** Virtual destructor */
		virtual ~AuthenticatorBase() = default;
	};
}

