For statements that execute often, you can prepare it to improve execution performance and reduce transfer size.<br/>
The preparation can be enabled globally or for a single query.<br/>

### Enable preparation globally

Use `setPrepareAllQueries(true)` in [SessionConfiguration](./SessionConfiguration.md#setprepareallqueriesbool), for example:

``` c++
auto configuration = cql::SessionConfiguration()
	.setPrepareAllQueries(true);
```

In this case, you can disable preparation for a single query by use `prepareQuery(false)` in `Command`, for example:

``` c++
auto command = cql::Command("select * from batchlog")
	.prepareQuery(false);
```

and `BatchCommand`, for example:

``` c++
auto command = cql::BatchCommand()
	.addQuery("update exmple_ks.my_table set name = ? where id = ?")
		.prepareQuery(false)
		.addParameters(cql::Text("aaa"), cql::Int(1));
```

### Enable preparation for a single query

By default, the preparation is disabled for all queries.<br/>
To enable preparation for a single query, use `prepareQuery(true)` just like above, for example:

``` c++
auto command = cql::Command("select * from batchlog")
	.prepareQuery();
```

and

``` c++
auto command = cql::BatchCommand()
	.addQuery("update exmple_ks.my_table set name = ? where id = ?")
		.prepareQuery()
		.addParameters(cql::Text("aaa"), cql::Int(1));
```

### More details

Unlike other drivers, this driver manages preparation fully automatic.<br/>
User doesn't have to manage the prepared id themselves, as a cost, every queries have to perform a hash table lookup by the query string.<br/>
But I think a hash table loopup is necessary anyway, because the prepared id is cached in each node separately.

The flow of preparation in this driver is like (python as pesudo language):

``` python
def sendQuery(connection, queryStr, needPrepare):
	if needPrepare:
		node = connection.node
		preparedId = node.preparedIdCache.get(queryStr)
		if preparedId is None:
			preparedId = connection.prepare(queryStr)
			node.preparedIdCache[queryStr] = preparedId
		executeMessage = ExecuteMessage(prepareid)
		return connection.send(executeMessage)
	else:
		queryMessage = QueryMessage(queryStr)
		return conection.send(queryMessage)
```

For a batch command, all queries need prepare will prepare in a pipeline, like this:

``` text
send PREPARE
send PREPARE
send PREPARE
receive RESULT
receive RESULT
receive RESULT
handle RESULT
handle RESULT
handle RESULT
```

Notice inappropriate setting of `SessionConfiguration::setMaxPendingMessages` may cause `max pending messages is reached` error, see documents [in there](./SessionConfiguration.md#setmaxpendingmessagesstdsize_t).

For more information about prepared statement see [this page](https://docs.datastax.com/en/developer/java-driver/3.1/manual/statements/prepared/).

