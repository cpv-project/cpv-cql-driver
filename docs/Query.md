After you create a `Session` instance from `SessionFactory`, you can perform database query by use the `Session::query` function.<br/>
The signature of `Session::query` is:

``` c++
seastar::future<ResultSet> query(Command&& command);
```

# Command

`Command` can built with chain style or command style:

``` c++
auto command = cql::Command("select * from batchlog")
	.setConsistency(cql::ConsistencyLevel::Quorum)
	.setPageSize(100);
```

Equals

``` c++
cql::Command command("select * from batchlog");
command.setConsistency(cql::ConsistencyLevel::Quorum);
command.setPageSize(100);
```

Then pass to `Session::query`:

``` c++
session.query(std::move(command)).then([] (cql::ResultSet result) {
	std::cout << "columns count: " << result.getColumnsCount() << std::endl;
	std::cout << "rows count: " << result.getRowsCount() << std::endl;
	cql::Int id;
	cql::Text name;
	for (std::size_t i = 0, j = result.getRowsCount(); i < j; ++i) {
		result.fill(id, name);
		std::cout << id << " " << name << std::endl;
	}
});
```

`Command` contains following options:

### setConsistency(ConsistencyLevel)

Set the consistency level of this query, default is "Quorum".<br/>
For more information see [this page](https://docs.datastax.com/en/cassandra/2.1/cassandra/dml/dml_config_consistency_c.html).<br/>

### setPageSize(std::size_t)

Set the page size of this query.<br/>
You may also want to set the page state if you want to query the next page.<br/>

### setPagingState(std::string&&)

Set the paging state of this query.<br/>
For the first page this is unnecessary.<br/>
Please sure you called the setPageSize before this function.<br/>

### addParameter(T&&)

Add single query parameter bound by position.<br/>
The position is incremental, when this function is called.<br/>

### addParameters(Args&&...)

Add multiple query parameters bound by position.<br/>
The position is incremental, when this function is called.<br/>

For example:

``` c++
auto command = cql::Command("insert into exmple_ks.my_table (id, name) values (?,?)")
	.addParameters(cql::Int(1), cql::Text("a"));
```

Equals

``` c++
auto command = cql::Command("insert into exmple_ks.my_table (id, name) values (?,?)")
	.addParameter(cql::Int(1))
	.addParameter(cql::Text("a"));
```

**Named parameter is unsupported, and it's not recommended to use.**

### setSerialConsistency(ConsistencyLevel)

Set the serial consistency level of this query.<br/>
Can only be either SERIAL or LOCAL_SERIAL.<br/>

### setDefaultTimestamp(std::chrono::system_clock::time_point)

Set the default timestamp of this query.<br/>
This will replace the server side assigned timestamp as default timestamp.<br/>
A timestamp in the query itself will still override this timestamp.<br/>

### setMaxRetries(std::size_t)

Set the maximum retry times *after* the first try has failed, default is 0.<br/>

# ResultSet

`ResultSet` contains the result of the query.<br/>
Notice `query` will return a empty(0 rows) `ResultSet` if the statement isn't select.<br/>

`ResultSet` contains following functions:

### bool isValid()

Check whether this is a valid result set (will be false if moved).

### std::size_t getRowsCount()

Get how many rows present in this result.

### std::size_t getColumnsCount()

Get how many columns selected by the query that produced this result.

### const std::string& getPagingState()

Get the value used to retrieve the next page of results.

### void fill(Args&... values)

Fill the values with specificed types.<br/>
The position is incremental, when this function is called.<br/>
Notice it won't check the actual type is matched.<br/>

For example, if you selected two columns and their types is Int and Text, you can use following code to get all records:

``` c++
cql::Int id;
cql::Text name;
for (std::size_t i = 0, j = result.getRowsCount(); i < j; ++i) {
	result.fill(id, name);
	std::cout << id << " " << name << std::endl;
}
```

**Get column index by it's name is unsupported, so don't use `select * from ...`.**

For more column types, see the document of [column types](./ColumnTypes.md).

