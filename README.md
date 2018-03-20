# C++ driver for Cassandra/Scylla based on seastar framework

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/ef5c262ba1674ccc9427630a09d4681a)](https://www.codacy.com/app/303248153/cpv-cql-driver?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=cpv-project/cpv-cql-driver&amp;utm_campaign=Badge_Grade)
[![license](https://img.shields.io/github/license/cpv-project/cpv-manage-scripts.svg)]() 

This is a cassandra/scylla driver written in c++ based on seastar framework, it's fast and easy to use.<br/>
For seastar framework please see [here](https://github.com/scylladb/seastar).<br/>
It's new and has not been extensively tested, you should use it carefully.<br/>

# Features

- [Futures based API](TODO)
- [Connection pooling](./docs/Configuration.md#setminpoolsizestdsize_t)
- [Connection multiplexing](./docs/Configuration.md#setmaxstreamsstdsize_t)
- [Automatic retry](TODO)
- [Load balancing](./docs/Configuration.md#nodecollection)
- [Authentication](./docs/Configuration.md#setpasswordauthenticationstdstring-stdstring)
- [SSL](./docs/Configuration.md#setusesslbool)
- [Batch Operation](TODO)

# Documents

- [Hello World](./docs/HelloWorld.md)
- [Configuration](./docs/Configuration.md)
- [Query](./docs/Query.md)
- [Execute](./docs/Execute.md)
- [Batch Execute](./docs/BatchExecute.md)
- [Column Types](./docs/ColumnTypes.md)
- [Logging](./docs/Logging.md)
- [Benchmark](./docs/Benchmark.md)

# Running tests

Here are the steps to run the unit tests, for now only ubuntu is supported.

``` text
git clone --recurse-submodules https://github.com/cpv-project/cpv-cql-driver
cd cpv-cql-driver
sh install-dependencies.sh
cd tests
sh prepare.sh
sh run_tests.sh
```

# Contribution

You should follow these rules when you are contributing code.

- Use tabs instead of spaces
- For class names, use camel case and start with a upper case (e.g. SomeClass)
- For function names, use camel case and start with a lower case (e.g. someFunction)
- For local variable names, use camel case and start with a lower case (e.g. someInt)
- For global variable names, use camel case and start with a upper case (e.g. SomeGlobalValue)
- For class member names, use camel case and start with a lower case and ends with `_` (e.g. someMember_)
- Write comments for every public class and function, keep the code as simple as possible

# License

LICENSE: MIT LICENSE<br/>
Copyright © 2017-2018 303248153@github<br/>
If you have any license issue please contact 303248153@qq.com.

