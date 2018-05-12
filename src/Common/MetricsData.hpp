#pragma once
#include <core/metrics_registration.hh>

namespace cql {
	/** Define metrics targets */
	class MetricsData {
	public:
		/** Constructor */
		MetricsData();

	public:
		/** The total number of connections opened */
		std::size_t pool_connections_total;
		/** The current number of open connections */
		std::size_t pool_connections_current;
		/** The total number of errors while initializing connections */
		std::size_t connection_initialize_errors;
		/** The total number of errors while sending messages */
		std::size_t connection_send_message_errors;
		/** The total number of errors while receiving messages */
		std::size_t connection_receive_message_errors;
		/** The total number of messages sent */
		std::size_t connection_messages_sent;
		/** The total number of messages received */
		std::size_t connection_messages_received;
		/** The total number of QUERY or EXECUTE command successfully executed */
		std::size_t session_command_successful;
		/** The total number of QUERY or EXECUTE command failed to execute */
		std::size_t session_command_failed;
		/** The total number of BATCH command successfully executed */
		std::size_t session_batch_command_successful;
		/** The total number of BATCH command failed to execute */
		std::size_t session_batch_command_failed;

	private:
		seastar::metrics::metric_groups groups_;
	};
}

