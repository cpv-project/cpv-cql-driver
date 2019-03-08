# Hello World

This document will teach you how to write a simple program that use this driver to manipulate a cassandra/scylla database.<br/>
First, if you don't known about seastar framework, you should read [this](https://github.com/scylladb/seastar/blob/master/doc/tutorial.md) before continue.

## Install cqldriver

Please follow the installation instructions on [README.md](../README.md),
to complete this tutorial, you should ensure following commands work.

- `pkg-config --cflags seastar`
- `pkg-config --libs seastar`
- `pkg-config --cflags cqldriver`
- `pkg-config --libs cqldriver`

If you install cqldriver from ubuntu ppa then both seastar and cqldriver will available as shared library,
static library from custom build should work too but I didn't test.

## A hello world program

Here is the source code of a hello world program:

``` c++
#include <iostream>
#include <seastar/core/app-template.hh>
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

To compile and run it, use following commands.

Notice the latest version (at 20181231) of seastar enables aio by default,
so if your environment doesn't support aio (like inside docker),
you should preserve the command line argument `--reactor-backend epoll`,
otherwise you could remove it.

``` text
g++ $(pkg-config --cflags seastar) \
	$(pkg-config --cflags cqldriver) \
	Main.cpp \
	$(pkg-config --libs seastar) \
	$(pkg-config --libs cqldriver)
./a.out --reactor-backend epoll
```

If you can see the following output that means you have successfully compile and run this program.

``` text
columns count: 4
rows count: 0
```

Now let's see what's in the source code.

### Include headers

The header `<seastar/core/app-template.hh>` is for running a seastar application,
a basic seastar application should like this:

``` c++
#include <seastar/core/app-template.hh>

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
Notice `SessionFactory` is not thread safe, **you should create a session factory for each thread(core)**.<br/>
Each core would have their own session factory and connection pool, it looks weird but that's the seastar way,<br/>
which not require thread locks and memory barriers at all.

### Create a session

Create a `Session` from `SessionFactory` is just one line:

``` c++
cql::Session session = sessionFactory.getSession();
```

Instance of `Session` is move only, you can't copy it.<br/>
**Don't execute commands in parallel with the same instance of Session**.</span>

### Create a command

A `Command` defines a database operation, the statement can be DML or DDL.
Create a `Command` is like this:

``` c++
cql::Command command("select * from batchlog");
```

You can use chain style or command style to specific options:

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

## Finally

Now I assume you already know how to write a simple program and compile it with this library.<br/>
You can read other documents for more information.<br/>
It's welcome to ask questions by open an issue on github.

