#include <CQLDriver/Common/Exceptions/NotImplementedException.hpp>
#include <CQLDriver/Common/CommonDefinitions.hpp>
#include "./AuthenticatorFactory.hpp"
#include "./AllowAllAuthenticator.hpp"
#include "./PasswordAuthenticator.hpp"

namespace cql {
	namespace AuthenticatorFactory {
		/** Get a suitable authenticator for specific node */
		seastar::shared_ptr<AuthenticatorBase> getAuthenticator(
			const NodeConfiguration& nodeConfiguration) {
			const auto& authenticatorClass = nodeConfiguration.getAuthenticatorClass();
			if (authenticatorClass.empty() ||
				authenticatorClass == AuthenticatorClasses::AllowAllAuthenticator) {
				static thread_local seastar::shared_ptr<AllowAllAuthenticator> authenticator =
					seastar::make_shared<AllowAllAuthenticator>();
				return authenticator;
			} else if (authenticatorClass == AuthenticatorClasses::PasswordAuthenticator) {
				static thread_local seastar::shared_ptr<PasswordAuthenticator> authenticator =
					seastar::make_shared<PasswordAuthenticator>();
				return authenticator;
			} else {
				throw NotImplementedException(CQL_CODEINFO,
					"unimplemented authenticator class:", authenticatorClass);
			}
		}
	}
}

