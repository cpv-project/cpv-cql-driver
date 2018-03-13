#pragma once
#include <vector>
#include <utility>
#include <core/queue.hh>
#include <CQLDriver/Common/Utility/Object.hpp>
#include <CQLDriver/Common/Utility/SocketHolder.hpp>
#include <CQLDriver/Common/SessionConfiguration.hpp>
#include <CQLDriver/Common/NodeConfiguration.hpp>
#include "./Connectors/ConnectorBase.hpp"
#include "./Authenticators/AuthenticatorBase.hpp"
#include "./RequestMessages/RequestMessageBase.hpp"
#include "./ResponseMessages/ResponseMessageBase.hpp"
#include "./ConnectionInfo.hpp"
#include "./ConnectionStream.hpp"

namespace cql {
	/**
	 * A single tcp connection to the database
	 * To send and receive message, a "Stream" object is required,
	 * Only one message can be sent or received at a time for the same stream.
	 */
	class Connection :
		public seastar::enable_lw_shared_from_this<Connection> {
	public:
		/** Get the session configuration used by this connection */
		const SessionConfiguration& getSessionConfiguration() const& { return *sessionConfiguration_; }

		/** Get the node configuration used by this connection */
		const NodeConfiguration& getNodeConfiguration() const& { return *nodeConfiguration_; }

		/** Get the connection information */
		const ConnectionInfo& getConnectionInfo() const& { return connectionInfo_; }

		/** Check whether this connection is ready */
		bool isReady() const { return isReady_; }

		/** Initialize connection and wait until it's ready to send ordinary messages */
		seastar::future<> ready();

		/**
		 * Open a new stream.
		 * If the no free stream available then a invalid stream object will be returned.
		 * Please don't forget to check stream.isValid().
		 */
		ConnectionStream openStream();

		/** Get how many free streams available */
		std::size_t getFreeStreamsCount() const;

		/** Check whether all streams are free */
		bool isAllStreamsFree() const;

		/** Send a message to the given stream and wait for success.  */
		seastar::future<> sendMessage(
			Object<RequestMessageBase>&& message,
			const ConnectionStream& stream);

		/** Send a message to the given stream and wait for success.  */
		template <class T,
			std::enable_if_t<std::is_base_of<RequestMessageBase, T>::value, int> = 0>
		seastar::future<> sendMessage(
			Object<T>&& message,
			const ConnectionStream& stream) {
			return sendMessage(std::move(message).template cast<RequestMessageBase>(), stream);
		}

		/**
		 * Wait for the next message from the given stream.
		 * Only one waiter can register at a time for each stream.
		 */
		seastar::future<Object<ResponseMessageBase>> waitNextMessage(
			const ConnectionStream& stream);

		/** Constructor */
		Connection(
			const seastar::lw_shared_ptr<SessionConfiguration>& sessionConfiguration,
			const seastar::lw_shared_ptr<NodeConfiguration>& nodeConfiguration);

		/** Constructor */
		Connection(
			const seastar::lw_shared_ptr<SessionConfiguration>& sessionConfiguration,
			const seastar::lw_shared_ptr<NodeConfiguration>& nodeConfiguration,
			const seastar::shared_ptr<ConnectorBase>& connector,
			const seastar::shared_ptr<AuthenticatorBase>& authenticator);

		/** Destructor */
		~Connection();

		/** Disallow copy and move */
		Connection(const Connection&) = delete;
		Connection(Connection&&) = delete;
		Connection& operator=(const Connection&) = delete;
		Connection& operator=(Connection&&) = delete;

	private:
		/** Close the connection */
		void close(const std::string& errorMessage);

	private:
		seastar::lw_shared_ptr<SessionConfiguration> sessionConfiguration_;
		seastar::lw_shared_ptr<NodeConfiguration> nodeConfiguration_;
		seastar::shared_ptr<ConnectorBase> connector_;
		seastar::shared_ptr<AuthenticatorBase> authenticator_;

		SocketHolder socket_;
		bool isReady_;
		ConnectionInfo connectionInfo_;

		seastar::lw_shared_ptr<std::vector<ConnectionStream::IdType>> freeStreamIds_;
		ConnectionStream streamZero_;

		seastar::future<> sendingFuture_;
		std::string sendingBuffer_;

		std::vector<std::pair<bool, seastar::promise<Object<ResponseMessageBase>>>> receivingPromiseMap_;
		std::vector<seastar::queue<Object<ResponseMessageBase>>> receivedMessageQueueMap_;
		std::size_t receivingPromiseCount_;
	};
}

