Since 0.4, this driver supports following metrics targets, you can check the example under `examples/Metrics` to see how to expose a prometheus interface.<br/>
You can also check the document of [metrics report](https://github.com/scylladb/seastar/wiki/Metrics-report) from seastar's repository.

# pool_connections_total

The total number of connections opened.

# pool_connections_current

The current number of open connections.

# connection_initialize_errors

The total number of errors while initializing connections.

# connection_send_message_errors

The total number of errors while sending messages.

# connection_receive_message_errors

The total number of errors while receiving messages.

# connection_messages_sent

The total number of messages sent.

# connection_messages_received

The total number of messages received.

# session_command_successful

The total number of QUERY or EXECUTE command successfully executed.

# session_command_failed

The total number of QUERY or EXECUTE command failed to execute.

# session_batch_command_successful

The total number of BATCH command successfully executed.

# session_batch_command_failed

The total number of BATCH command failed to execute.
