If you want to perform multiple data modification operations in a single roundtrip, `Session::batchExecute` will be the choice.<br/>
The signature of `Session::batchExecute` is:

``` c++
seastar::future<> batchExecute(BatchCommand&& command);
```

Different to `query` and `execute`, `batchExecute` takes `BatchCommand` instead of `Command`.<br/>
Only insert, update and delete statements can use in a batch command.<br/>

# BatchCommand

`BatchCommand` can built with chain style or command style:

``` c++
auto command = cql::BatchCommand()
	.addQuery("insert into exmple_ks.my_table (id, name) values (?,?)")
		.prepareQuery()
		.openParameterSet()
			.addParameters(cql::Int(1), cql::Text("a"))
		.openParameterSet()
			.addParameters(cql::Int(2), cql::Text("b"))
		.openParameterSet()
			.addParameters(cql::Int(3), cql::Text("c"))
	.addQuery("update exmple_ks.my_table set name = ? where id = ?")
		.addParameters(cql::Text("aaa"), cql::Int(1))
	.addQuery("delete from exmple_ks.my_table where id = ?")
		.addParameters(cql::Int(3));
```

Equals

``` c++
cql::BatchCommand command;
command.addQuery("insert into exmple_ks.my_table (id, name) values (?,?)")
command.prepareQuery();
command.openParameterSet();
command.addParameters(cql::Int(1), cql::Text("a"));
command.openParameterSet();
command.addParameters(cql::Int(2), cql::Text("b"));
command.openParameterSet();
command.addParameters(cql::Int(3), cql::Text("c"));
command.addQuery("update exmple_ks.my_table set name = ? where id = ?");
command.addParameters(cql::Text("aaa"), cql::Int(1));
command.addQuery("delete from exmple_ks.my_table where id = ?");
command.addParameters(cql::Int(3));
```

Then pass to `Session::batchExecute`:

``` c++
session.batchExecute(std::move(command)).then([] {
	std::cout << "batch was successful" << std::endl;
});
```

`BatchCommand` contains following options:

### setType(BatchType)

Set the type of this batch, default is "Logged".<br/>
Supported batch types:<br/>

- BatchType::Logged
- BatchType::UnLogged
- BatchType::Counter

### setConsistency(ConsistencyLevel)

Set the consistency level of this query, default is "Quorum".<br/>
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

### addQuery(...)

Add a new query to this batch.
There multiple overloads of this function:

``` c++
BatchCommand& addQuery(std::string&& query) &;
BatchCommand& addQuery(const char* query, std::size_t size) &;
template <std::size_t Size>
BatchCommand& addQuery(const char(&query)[Size]) &;
```

Notice overload 2 and 3 won't copy the string to the internal buffer in `BatchCommand`,
it can avoid the excess memory copy,
but you have to ensure the string is alive until `Session::batchExecute` is finished.

### prepareQuery()

Prepare the last query to reduce the message size.<br/>
Notice it will increase one roundtrip.<br/>

**For now this driver won't rememeber prepared query, the query will be prepared every time the batch command is executed.**
I want to implement this feature but I don't know what to do if a node has restarted, detect it by receives events may be unreliable.

### openParameterSet()

Open a new parameter set explicitly of the last query.<br/>
This function is optional if there only one parameter set, for example:<br/>

``` c++
auto command = cql::BatchCommand()
	.addQuery("update exmple_ks.my_table set name = ? where id = ?")
		.addParameters(cql::Text("aaa"), cql::Int(1));
```

Euqals

``` c++
auto command = cql::BatchCommand()
	.openParameterSet()
		.addQuery("update exmple_ks.my_table set name = ? where id = ?")
			.addParameters(cql::Text("aaa"), cql::Int(1));
```

But it's required if you want to open multiple parameter sets, for example:

``` c++
auto command = cql::BatchCommand()
	.addQuery("insert into exmple_ks.my_table (id, name) values (?,?)")
		.prepareQuery()
		.openParameterSet()
			.addParameters(cql::Int(1), cql::Text("a"))
		.openParameterSet()
			.addParameters(cql::Int(2), cql::Text("b"))
		.openParameterSet()
			.addParameters(cql::Int(3), cql::Text("c"));
```

### addParameter(T&& parameter)

Add single query parameter bound by position to the last parameter set.<br/>
The position is incremental, when this function is called.<br/>

### addParameters(Args&&...)

Add multiple query parameters bound by position to the last parameter set.<br/>
The position is incremental, when this function is called.<br/>

For example:

``` c++
auto command = cql::BatchCommand()
	.addQuery("update exmple_ks.my_table set name = ? where id = ?")
		.addParameters(cql::Text("aaa"), cql::Int(1));
```

Equals

``` c++
auto command = cql::BatchCommand()
	.addQuery("update exmple_ks.my_table set name = ? where id = ?")
		.addParameter(cql::Text("aaa"))
		.addParameter(cql::Int(1));
```

### setSerialConsistency(ConsistencyLevel)

Set the serial consistency level of this query.<br/>
Can only be either SERIAL or LOCAL_SERIAL.<br/>

### setDefaultTimestamp(std::chrono::system_clock::time_point)

Set the default timestamp of this query.<br/>
This will replace the server side assigned timestamp as default timestamp.<br/>
A timestamp in the query itself will still override this timestamp.<br/>

### setMaxRetries(std::size_t)

Set the maximum retry times *after* the first try has failed, default is 0.<br/>
**Please ensure all statements are idempotent, this driver won't check it.**<br/>

