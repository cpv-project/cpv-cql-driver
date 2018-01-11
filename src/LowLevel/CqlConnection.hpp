#pragma once
#include <CqlDriver/Common/Utility/CqlObject.hpp>
#include <CqlDriver/Common/CqlSessionConfiguration.hpp>
#include <CqlDriver/Common/CqlNodeConfiguration.hpp>
#include "./Connectors/CqlConnectorBase.hpp"
#include "./Authenticators/CqlAuthenticatorBase.hpp"
#include "./RequestMessages/CqlRequestMessageBase.hpp"
#include "./ResponseMessages/CqlResponseMessageBase.hpp"

namespace cql {
	/** A single tcp connection to the database */
	class CqlConnection :
		public seastar::enable_shared_from_this<CqlConnection> {
	public:
		/** Representing an in use stream */
		class Stream {
		public:
			struct State { bool isInUse; };
			std::uint16_t getStreamId() const;
			bool isValid() const;
			Stream(std::uint16_t streamId, const seastar::lw_shared_ptr<State>& state);
			~Stream();
			Stream(const Stream&) = delete;
			Stream& operator=(const Stream&) = delete;
			Stream(Stream&& stream);
			Stream& operator=(Stream&& stream);

		private:
			std::uint16_t streamId_;
			seastar::lw_shared_ptr<State> state_;
		};

		/** Initialize connection and wait until it's ready to send ordinary messages */
		seastar::future<> ready();

		/** TODO */
		Stream openStream();

		/** TODO */
		seastar::future<> sendMessage(CqlObject<CqlRequestMessageBase>&& message, const Stream& stream);

		/** TODO */
		seastar::future<CqlObject<CqlResponseMessageBase>> waitNextMessage(const Stream& stream);

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
		/** TODO */
		void startSender();

		/** TODO */
		void startReceiver();

	private:
		seastar::shared_ptr<CqlSessionConfiguration> sessionConfiguration_;
		seastar::shared_ptr<CqlNodeConfiguration> nodeConfiguration_;
		seastar::shared_ptr<CqlConnectorBase> connector_;
		seastar::shared_ptr<CqlAuthenticatorBase> authenticator_;

		seastar::connected_socket socket_;
		bool isReady_;
	};
}

