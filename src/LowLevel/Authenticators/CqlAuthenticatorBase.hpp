#pragma once
#include <core/future.hh>

namespace cql {
	class CqlConnection;

	/** The interface used to perform the authentication on cql database connection */
	class CqlAuthenticatorBase {
	public:
		/** Perform the authentication */
		virtual seastar::future<> authenticate(CqlConnection&) const = 0;

		/** Virtual destructor */
		virtual ~CqlAuthenticatorBase() = default;
	};
}

