#include <vector>
#include <seastar/core/metrics.hh>
#include <CQLDriver/Common/Utility/StringUtils.hpp>
#include "MetricsData.hpp"

namespace cql {
	/** Constructor */
	MetricsData::MetricsData() :
		pool_connections_total(0),
		pool_connections_current(0),
		connection_initialize_errors(0),
		connection_send_message_errors(0),
		connection_receive_message_errors(0),
		connection_messages_sent(0),
		connection_messages_received(0),
		session_command_successful(0),
		session_command_failed(0),
		session_batch_command_successful(0),
		session_batch_command_failed(0),
		groups_() {
		static thread_local std::size_t ServiceId = 0;
		std::vector<seastar::metrics::label_instance> labels;
		labels.emplace_back("service", joinString("", "cql-driver-", ServiceId++));
		groups_.add_group("cql-driver", {
			seastar::metrics::make_derive(
				"pool_connections_total",
				[this] { return pool_connections_total; },
				seastar::metrics::description("The total number of connections opened"),
				labels),
			seastar::metrics::make_gauge(
				"pool_connections_current",
				[this] { return pool_connections_current; },
				seastar::metrics::description("The current number of open connections"),
				labels),
			seastar::metrics::make_derive(
				"connection_initialize_errors",
				[this] { return connection_initialize_errors; },
				seastar::metrics::description("The total number of errors while initializing connections"),
				labels),
			seastar::metrics::make_derive(
				"connection_send_message_errors",
				[this] { return connection_send_message_errors; },
				seastar::metrics::description("The total number of errors while sending messages"),
				labels),
			seastar::metrics::make_derive(
				"connection_receive_message_errors",
				[this] { return connection_receive_message_errors; },
				seastar::metrics::description("The total number of errors while receiving messages"),
				labels),
			seastar::metrics::make_derive(
				"connection_messages_sent",
				[this] { return connection_messages_sent; },
				seastar::metrics::description("The total number of messages sent"),
				labels),
			seastar::metrics::make_derive(
				"connection_messages_received",
				[this] { return connection_messages_received; },
				seastar::metrics::description("The total number of messages received"),
				labels),
			seastar::metrics::make_derive(
				"session_command_successful",
				[this] { return session_command_successful; },
				seastar::metrics::description(
					"The total number of QUERY or EXECUTE command successfully executed"),
				labels),
			seastar::metrics::make_derive(
				"session_command_failed",
				[this] { return session_command_failed; },
				seastar::metrics::description(
					"The total number of QUERY or EXECUTE command failed to execute"),
				labels),
			seastar::metrics::make_derive(
				"session_batch_command_successful",
				[this] { return session_batch_command_successful; },
				seastar::metrics::description("The total number of BATCH command successfully executed"),
				labels),
			seastar::metrics::make_derive(
				"session_batch_command_failed",
				[this] { return session_batch_command_failed; },
				seastar::metrics::description("The total number of BATCH command failed to execute"),
				labels)
		});
	}
}

