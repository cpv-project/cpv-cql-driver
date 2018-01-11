#include <CqlDriver/Common/Exceptions/CqlNotImplementedException.hpp>
#include <CqlDriver/Common/CqlCommonDefinitions.hpp>
#include "CqlAuthenticatorFactory.hpp"
#include "CqlAllowAllAuthenticator.hpp"
#include "CqlPasswordAuthenticator.hpp"

namespace cql {
	namespace CqlAuthenticatorFactory {
		/** Get a suitable authenticator for specific node */
		seastar::shared_ptr<CqlAuthenticatorBase> getAuthenticator(
			const CqlNodeConfiguration& nodeConfiguration) {
			const auto& authenticatorClass = nodeConfiguration.getAuthenticatorClass();
			if (authenticatorClass.empty() ||
				authenticatorClass == CqlAuthenticatorClasses::AllowAllAuthenticator) {
				static thread_local seastar::shared_ptr<CqlAllowAllAuthenticator> authenticator =
					seastar::make_shared<CqlAllowAllAuthenticator>();
				return authenticator;
			} else if (authenticatorClass == CqlAuthenticatorClasses::PasswordAuthenticator) {
				static thread_local seastar::shared_ptr<CqlPasswordAuthenticator> authenticator =
					seastar::make_shared<CqlPasswordAuthenticator>();
				return authenticator;
			} else {
				throw CqlNotImplementedException(CQL_CODEINFO,
					"unimplemented authenticator class:", authenticatorClass);
			}
		}
	}
}

