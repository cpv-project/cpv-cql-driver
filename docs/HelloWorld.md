This document will teach you how to write a simple program that use this driver to manipulate a cassandra/scylla database.<br/>
First, if you don't known about seastar framework, you should read [this](https://github.com/scylladb/seastar/blob/master/doc/tutorial.md) before continue.

# Download and compile

The first thing we need to do is download this library, use this command:

``` text
git clone --recurse-submodules ssh://git@github.com/cpv-project/cpv-cql-driver
```

Then install the packages that required to build it:

``` text
cd cpv-cql-driver
sudo sh install-dependencies.sh
```

To start the build, use this command:

``` text
sh build-release.sh
```

After the build is completed you should see `libCQLDriver.a` and `cqldriver.pc` under `bin/release`.<br/>
That's the files we need to use this library.

# A hello world program

Here is the source code of a hello world program:

``` c++
#include <iostream>
#include <core/app-template.hh>
#include <CQLDriver/CQLDriver.hpp>

int main(int argc, char** argv) {
	seastar::app_template app;
	app.run(argc, argv, [] {
		cql::SessionFactory sessionFactory(
			cql::SessionConfiguration()
				.setDefaultKeySpace("system"),
			cql::NodeCollection::create({
				cql::NodeConfiguration()
					.setAddress("127.0.0.1", 9043)
			}));
		cql::Session session = sessionFactory.getSession();
		cql::Command command("select * from batchlog");
		return session.query(std::move(command)).then([] (cql::ResultSet result) {
			std::cout << "columns count: " << result.getColumnsCount() << std::endl;
			std::cout << "rows count: " << result.getRowsCount() << std::endl;
		});
	});
	return 0;
}
```

To compile and run it, use these commands, replace `/path/to/cqldriver.pc` to the actual path:

``` text
g++ $(pkg-config --cflags /path/to/cqldriver.pc) Main.cpp $(pkg-config --libs /path/to/cqldriver.pc)
./a.out
```

If you can see the following output that means you have successfully compile and run this program.

``` text
columns count: 4
rows count: 0
```

Now let's see what's in the source code.

### Include headers

The header `<core/app-template.hh>` is for running a seastar application, a basic seastar application should like this:

``` c++
#include <core/app-template.hh>

int main(int argc, char** argv) {
	seastar::app_template app;
	app.run(argc, argv, [] {
		return seastar::make_ready_future<>();
	});
	return 0;
}
```

The header `<CQLDriver/CQLDriver.hpp>` is for using this driver, this is an all-in-one header.<br/>
If you only want to define data models instead of manipulating them, you can just include `<CQLDriver/CQLColumnTypes.hpp>`, that's much smaller.<br/>
And if you really don't want to include extra header files, you can just include the parts you need like this:

``` c++
#include <CQLDriver/Common/ColumnTypes/Int.hpp>
#include <CQLDriver/Common/ColumnTypes/Text.hpp>
#include <CQLDriver/Common/Command.hpp>
#include <CQLDriver/HighLevel/Session.hpp>
#include <CQLDriver/HighLevel/SessionFactory.hpp>
```

### Create a session factory

A `Session` is a object used to manipulate the database, to create a `Session`, you need a `SessionFactory`.
Create `SessionFactory` requires 2 parameters, `SessionConfiguration` and `NodeCollection`.
The code is like this:

``` c++
cql::SessionFactory sessionFactory(
	cql::SessionConfiguration()
		.setDefaultKeySpace("system"),
	cql::NodeCollection::create({
		cql::NodeConfiguration()
			.setAddress("127.0.0.1", 9043)
	}));
```

You can see the other options of `SessionConfiguration` and `NodeConfiguration` in the [next chapter](./Configuration.md).<br/>
Instance of `SessionFactory` is reference counted, you can copy it as you like.<br/>
Notice `SessionFactory` is not thread safe, <span style="color:red">you should create a session factory for each thread(core)</span>.<br/>
Each core would have their own session factory and connection pool, it looks weird but that's the seastar way,<br/>
which not require thread locks and memory barriers at all.

### Create a session

Create a `Session` from `SessionFactory` is just one line:

``` c++
cql::Session session = sessionFactory.getSession();
```

Instance of `Session` is move only, you can't copy it.<br/>
<span style="color:red">Don't execute commands in parallel with the same instance of Session.</span>

### Create a command

A `Command` defines a database operation, the statement can be DML or DDL.
Create a `Command` is like this:

``` c++
cql::Command command("select * from batchlog");
```

You can use the chain style or the usual style to specific options:

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

Instance of `Command` is move only, you can't copy it.<br/>
For more information please read the documents about [Query](./Query.md), [Execute](./Execute.md) and [BatchExecute](./BatchExecute.md).

### Execute a command

There two way to execute a command, one is `Session::query` and the other is `Session::execute`.<br/>
`Session::query` is for select statement and `Session::execute` is for other statements.<br/>
Execute a comand will return a `seastar::future`, like this:<br/>

``` c++
session.query(std::move(command)).then([] (cql::ResultSet result) {
	std::cout << "columns count: " << result.getColumnsCount() << std::endl;
	std::cout << "rows count: " << result.getRowsCount() << std::endl;
});
```

The return type of `Session::query` is `seastar::future<ResultSet>`.<br/>
And the return type of of `Session::execute` is `seastar::future<>`.

### Dynamic linking

For now this library doesn't support dynamic linking, so the linking time is long.<br/>
To reduce the executable binary size you could use the `strip` command.<br/>
I will provide dynamic linking support in the future version, and you can contribute if you already did it.

# Finally

Now I assume you already know how to write a simple program and compile it with this library.<br/>
You can read other documents for more information.<br/>
It's welcome to ask questions by open an issue on github.

