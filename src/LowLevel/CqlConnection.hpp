#pragma once
#include <memory>
#include <net/api.hh>
#include <core/reactor.hh>
#include <CqlDriver/Common/CqlSessionConfiguration.hpp>
#include <CqlDriver/Common/CqlNodeConfiguration.hpp>
#include "./Connectors/CqlConnectorBase.hpp"
#include "./Authenticators/CqlAuthenticatorBase.hpp"

namespace cql {
	/**
	 * Map to single tcp connection to the database
	 * Example:
	 * TODO
	 */
	class CqlConnection :
		public seastar::enable_shared_from_this<CqlConnection> {
	public:
		/** Representing an in use stream */
		/* class Stream {
		public:
			std::uint16_t getStreamId() const { return streamId_; }
			Stream(std::uint16_t streamId, seastar::weak_ptr<CqlConnection> connection) :
				streamId_(streamId), connection_(connection) { }
			~Stream();
			Stream(const Stream&) = delete;
			Stream& operator=(const Stream&) = delete;

		private:
			std::uint16_t streamId_;
			seastar::weak_ptr<CqlConnection> connection_;
		}; */

		/** Initialize connection and wait until it's ready to send ordinary messages */
		seastar::future<> ready();

		// openStream

		// sendMessage

		// waitNextMessage

		/** Constructor */
		CqlConnection(
			const seastar::shared_ptr<CqlSessionConfiguration>& sessionConfiguration,
			const seastar::shared_ptr<CqlNodeConfiguration>& nodeConfiguration);

		/** Constructor */
		CqlConnection(
			const seastar::shared_ptr<CqlSessionConfiguration>& sessionConfiguration,
			const seastar::shared_ptr<CqlNodeConfiguration>& nodeConfiguration,
			const seastar::shared_ptr<CqlConnectorBase>& connector,
			const seastar::shared_ptr<CqlAuthenticatorBase>& authenticator);

	private:
		// startSendLoop (should not do it)

		// startReceiveLoop (should not do it)

	private:
		seastar::shared_ptr<CqlSessionConfiguration> sessionConfiguration_;
		seastar::shared_ptr<CqlNodeConfiguration> nodeConfiguration_;
		seastar::shared_ptr<CqlConnectorBase> connector_;
		seastar::shared_ptr<CqlAuthenticatorBase> authenticator_;

		seastar::connected_socket socket_;
		bool isReady_;
	};
}

