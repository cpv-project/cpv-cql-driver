`Session::execute` is almost the same as `Session::query`, the difference is `execute` won't return `ResultSet`.<br/>
The signature of `Session::execute` is:

``` c++
seastar::future<> execute(Command&& command);
```

For example:

``` c++
auto command = cql::Command("insert into exmple_ks.my_table (id, name) values (?,?)")
	.addParameters(cql::Int(1), cql::Text("a"));
session.execute(std::move(command)).then([] {
	std::cout << "insert was successful" << std::endl;
});
```

**`Session::execute` can take select statement even it's meanless, and `Session::query` can take insert or update statements.**
In other words, if you don't care a slight difference in performance, you can always use `Session::query` to perform database operations.

