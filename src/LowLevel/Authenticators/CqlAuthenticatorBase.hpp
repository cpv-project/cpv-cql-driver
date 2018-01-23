#pragma once
#include <core/future.hh>
#include <core/shared_ptr.hh>

namespace cql {
	class CqlConnection;
	class CqlConnectionStream;

	/** The interface used to perform the authentication on cql database connection */
	class CqlAuthenticatorBase {
	public:
		/** Perform the authentication */
		virtual seastar::future<> authenticate(
			const seastar::shared_ptr<CqlConnection>& connection,
			const CqlConnectionStream& stream) const = 0;

		/** Virtual destructor */
		virtual ~CqlAuthenticatorBase() = default;
	};
}

