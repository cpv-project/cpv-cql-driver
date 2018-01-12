#pragma once
#include <vector>
#include <utility>
#include <core/queue.hh>
#include <CqlDriver/Common/Utility/CqlObject.hpp>
#include <CqlDriver/Common/CqlSessionConfiguration.hpp>
#include <CqlDriver/Common/CqlNodeConfiguration.hpp>
#include "./Connectors/CqlConnectorBase.hpp"
#include "./Authenticators/CqlAuthenticatorBase.hpp"
#include "./RequestMessages/CqlRequestMessageBase.hpp"
#include "./ResponseMessages/CqlResponseMessageBase.hpp"
#include "CqlConnectionInfo.hpp"

namespace cql {
	/**
	 * A single tcp connection to the database
	 * To send and receive message, a "Stream" object is required,
	 * Only one message can be sent or received at a time for the same stream.
	 */
	class CqlConnection :
		public seastar::enable_shared_from_this<CqlConnection> {
	public:
		/** Representing an in use stream */
		class Stream {
		public:
			struct State { bool isInUse; State() : isInUse(false) { } };
			std::uint16_t getStreamId() const { return streamId_; }
			bool isValid() const { return state_.get() != nullptr; }
			Stream(std::uint16_t streamId, const seastar::lw_shared_ptr<State>& state);
			Stream(const Stream&) = delete;
			Stream& operator=(const Stream&) = delete;
			Stream(Stream&& stream);
			Stream& operator=(Stream&& stream);
			~Stream();

		private:
			std::uint16_t streamId_;
			seastar::lw_shared_ptr<State> state_;
		};

		/** Initialize connection and wait until it's ready to send ordinary messages */
		seastar::future<> ready();

		/**
		 * Open a new stream.
		 * If the maximum numbers of streams have reached a invalid Stream object will be returned.
		 * Please don't forget to check stream.isValid().
		 */
		Stream openStream();

		/**
		 * Send a message to the given stream and wait for success.
		 * Only one message can be sent at a time for the same stream.
		 */
		seastar::future<> sendMessage(CqlObject<CqlRequestMessageBase>&& message, const Stream& stream);

		/**
		 * Wait for the next message from the given stream.
		 * Only one message can be received at a time for the same stream.
		 */
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
		/**
		 * Start background message sender.
		 * Only one sender can run at the same time, and it will exit after jobs are finished.
		 */
		void startSender();

		/**
		 * Start background message receiver.
		 * Only one receiver and run at the same time, and it will exit after jobs are finished.
		 */
		void startReceiver();

	private:
		seastar::shared_ptr<CqlSessionConfiguration> sessionConfiguration_;
		seastar::shared_ptr<CqlNodeConfiguration> nodeConfiguration_;
		seastar::shared_ptr<CqlConnectorBase> connector_;
		seastar::shared_ptr<CqlAuthenticatorBase> authenticator_;

		seastar::connected_socket socket_;
		bool isReady_;
		CqlConnectionInfo connectionInfo_;

		std::vector<seastar::lw_shared_ptr<Stream::State>> streamStates_;
		Stream streamZero_;
		std::size_t lastOpenedStream_;

		seastar::queue<seastar::promise<>> sendPromiseQueue_;
		std::vector<std::pair<bool, seastar::promise<>>> sendPromiseMap_;
		std::size_t receivePromiseCount_;
		std::vector<std::pair<bool, seastar::promise<CqlObject<CqlResponseMessageBase>>>> receivePromiseMap_;
	};
}

