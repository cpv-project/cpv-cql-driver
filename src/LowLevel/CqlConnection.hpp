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

		/** Send a message to the given stream and wait for success.  */
		seastar::future<> sendMessage(CqlObject<CqlRequestMessageBase>&& message, const Stream& stream);

		/**
		 * Wait for the next message from the given stream.
		 * Only one waiter can register at a time for each stream.
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

		/** Destructor */
		~CqlConnection();

		/** Disallow copy and move */
		CqlConnection(const CqlConnection&) = delete;
		CqlConnection(CqlConnection&&) = delete;
		CqlConnection& operator=(const CqlConnection&) = delete;
		CqlConnection& operator=(CqlConnection&&) = delete;

	private:
		/** Close the connection */
		void close(const seastar::sstring& errorMessage);

	private:
		seastar::shared_ptr<CqlSessionConfiguration> sessionConfiguration_;
		seastar::shared_ptr<CqlNodeConfiguration> nodeConfiguration_;
		seastar::shared_ptr<CqlConnectorBase> connector_;
		seastar::shared_ptr<CqlAuthenticatorBase> authenticator_;

		seastar::connected_socket socket_;
		seastar::input_stream<char> readStream_;
		seastar::output_stream<char> writeStream_;
		bool isReady_;
		CqlConnectionInfo connectionInfo_;

		std::vector<seastar::lw_shared_ptr<Stream::State>> streamStates_;
		Stream streamZero_;
		std::size_t lastOpenedStream_;

		seastar::future<> sendingFuture_;
		seastar::sstring sendingBuffer_;

		std::vector<std::pair<bool, seastar::promise<CqlObject<CqlResponseMessageBase>>>> receivingPromiseMap_;
		std::vector<seastar::queue<CqlObject<CqlResponseMessageBase>>> receivedMessageQueueMap_;
		std::size_t receivingPromiseCount_;
	};
}

