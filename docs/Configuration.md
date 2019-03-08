# Configuration

To create a `SessionFactory`, two types of configuration is needed. `SessionConfiguration` provides the common configurations for all sessions and all connections, `NodeConfiguration` provides node specified configurations for a single node and the connections connected to this node.

## SessionConfiguration

`SessionConfiguration` can built with chain style or command style:

``` c++
auto configuration = cql::SessionConfiguration()
	.setMinPoolSize(1)
	.setMaxPoolSize(100)
	.setDefaultKeySpace("system");
```

Equals

``` c++
cql::SessionConfiguration configuration;
configuration.setMinPoolSize(1);
configuration.setMaxPoolSize(100);
configuration.setDefaultKeySpace("system");
```

`SessionConfiguration` contains following options:

### `setMinPoolSize(std::size_t)`

Set the minimum database connection pool size.<br/>
Notice it's per core configuration.<br/>
The default value is 5.<br/>

### `setMaxPoolSize(std::size_t)`

Set the maximum database connection pool size.<br/>
Notice it's per core configuration,<br/>
for example if the max pool size is 100 and there 2 cpu cores,<br/>
then the maximum database connections in the pool for a single process is 200.<br/>
The default value is 100.<br/>

### `setMaxWaitersAfterConnectionsExhausted(std::size_t)`

Set how many waiters can exist after connections are exhausted.<br/>
After all connections are occupied and the poll can't raise new connection,<br/>
the one wants to acquire connection from the pool has to wait,<br/>
this set how many waiters can exist in such situation,<br/>
if the maximum number of waitiers has been reached,<br/>
the pool will return an error instead of waiting.<br/>
Notice it's per core configuration.<br/>
The default value is 100.<br/>

### `setDnsCacheTime(std::chrono::milliseconds)`

Set the dns cache time for the hostname of nodes.<br/>
The default value is 30000ms.<br/>

### `setDefaultKeySpace(const std::string&)`

Set the default keyspace of all connections created by this configuration.<br/>

### `setDefaultConsistency(ConsistencyLevel)`

Set the default consistency level.<br/>
The default value is `ConsistencyLevel.Quorum`.<br/>
This can be overridden by `setConsistency(...)` in [Command](./Query.md#setconsistencyconsistencylevel) and [BatchCommand](./BatchExecute.md#setconsistencyconsistencylevel).<br/>
For more information see [this page](https://docs.datastax.com/en/cassandra/2.1/cassandra/dml/dml_config_consistency_c.html).<br/>
Supported consistencies:<br/>

- ConsistencyLevel::Any
- ConsistencyLevel::One
- ConsistencyLevel::Two
- ConsistencyLevel::Three
- ConsistencyLevel::Quorum
- ConsistencyLevel::All
- ConsistencyLevel::LocalQuorum
- ConsistencyLevel::EachQuorum
- ConsistencyLevel::Serial
- ConsistencyLevel::LocalSerial
- ConsistencyLevel::LocalOne

### `setPrepareAllQueries(bool)`

Set should prepare all queries by default.<br/>
The default value is false.<br/>
This can be overridden by `prepareQuery(bool)` in [Command](./Query.md#preparequerybool) and [BatchCommand](./BatchExecute.md#preparequerybool).

### `setLogger(seastar::shared_ptr<Logger>&)`

Set the logger instance.<br/>
See the document about [Logging](./Logging.md).

## NodeConfiguration

`NodeConfiguration` can built with chain style or command style:

``` c++
auto node = cql::NodeConfiguration()
	.setAddress("127.0.0.1", 9043)
	.setUseSSL(true);
```

Equals

``` c++
cql::NodeConfiguration node;
node.setAddress("127.0.0.1", 9043);
node.setUseSSL(true);
```

`NodeConfiguration` contains following options:

### `setAddress(std::string&&, std::uint16_t)`

Set the hostname and the port of this node.<br/>

### `setUseSSL(bool)`

Set should connect this node with ssl connection, default value is false.<br/>

### `setUseCompression(bool)`

Set should use frame compression if available, default value is false.<br/>
For now only lz4 algorithm is supported and will be used if this option is set.

### `setMaxStreams(std::size_t)`

Set how many streams can hold in a single connection, default value is 20.<br/>

### `setMaxPendingMessages(std::size_t)`

Set how many messages can hold in a received queue for a single stream, default value is 100.<br/>
Notice this setting may cause batch execute with preparation failed,<br/>
for example if max pending messages is 100 and there 101 queries to prepare,<br/>
you may see an error like `max pending messages is reached`,<br/>
the solution is increase this limitation if you have too many queries in a single batch.

### `setPasswordAuthentication(std::string&&, std::string&&)`

Set to use password authentication for this node.<br/>
The first argument is username, the second argument is password.

### `setKeepaliveParameters(std::optional<seastar::net::keepalive_params>&& keepaliveParameters)`

Set keepalive parameters of connection, default is use kernel settings which can modify by sysctl.<br/>
By default all connection will enable tcp keepalive to avoid dead connection persistent.

## Create a session factory

Here is the code to create a session factory:

``` c++
auto configuration = cql::SessionConfiguration()
	.setMinPoolSize(1)
	.setMaxPoolSize(100)
	.setDefaultKeySpace("system");
auto nodeA = cql::NodeConfiguration()
	.setAddress("127.0.0.1", 9043)
	.setUseSSL(true);
auto nodeB = cql::NodeConfiguration()
	.setAddress("127.0.0.1", 9044)
	.setUseSSL(true);
cql::SessionFactory sessionFactory(
	configuration,
	cql::NodeCollection::create({ nodeA, nodeB }));
```

`NodeCollection` is an interface use to manage node configurations and choose node based on some strategy, you can implement your own node collection to do the custom load balancing.

## NodeCollection

The default implemention of `NodeCollection` is very simple, it will choose nodes in order, if connect to a node has failed, the node will be put on a fault list, nodes in fault list will not be choose unless a round has finished.<br/>
For example, there nodes A, B, C, D, E:

- Round 1
	- Choose node A
		- Connect to node A was successful
	- Choose node B
		- Connect to node B was successful
	- Choose node C
		- Connect to node C has failed
		- Put node C to fault list
	- Choose node D
		- Connect to node D has failed
		- Put node D to fault list
	- Choose node E
		- Connect to node E was successful
- Round 2
	- Try to recover one fault node after each times all nodes has been enumerated
	- Choose node C
		- Connect to node C has failed
	- Choose node A
	- Choose node B
	- Choose node E
- Round 3
	- Try to recover one fault node after each times all nodes has been enumerated
	- Choose node D
		- Connect to node D was successful
		- Remove node D from fault list
	- Choose node A
	- Choose node B
	- Choose node D
	- Choose node E

For now it doesn't care about data-center and will not auto discover newly joined nodes, I may implement them in the future version, in the mean time you can implement them yourself and replace the instance of `NodeCollection` pass to `SessionFactory`.

