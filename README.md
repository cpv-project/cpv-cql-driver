# C++ driver for Cassandra/Scylla based on seastar framework

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/a5c9af91941a4b8d8131068d8dfb76f1)](https://www.codacy.com/app/compiv/cpv-cql-driver?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=cpv-project/cpv-cql-driver&amp;utm_campaign=Badge_Grade)
[![Build Status](https://travis-ci.org/cpv-project/cpv-cql-driver.svg?branch=master)](https://travis-ci.org/cpv-project/cpv-cql-driver)
[![license](https://img.shields.io/github/license/cpv-project/cpv-manage-scripts.svg)]() 
[![GitHub release](https://img.shields.io/github/release/cpv-project/cpv-cql-driver.svg)]()

This is a cassandra/scylla driver written in c++ based on seastar framework, it's fast and easy to use.<br/>
For seastar framework please see [here](https://github.com/scylladb/seastar).<br/>
It's new and has not been extensively tested, you should use it carefully.<br/>

### Features

- [Futures based API](./docs/HelloWorld.md)
- [Connection pooling](./docs/Configuration.md#setminpoolsizestdsize_t)
- [Connection multiplexing](./docs/Configuration.md#setmaxstreamsstdsize_t)
- [Automatic retry](./docs/Query.md#setmaxretriesstdsize_t)
- [Load balancing](./docs/Configuration.md#nodecollection)
- [Authentication](./docs/Configuration.md#setpasswordauthenticationstdstring-stdstring)
- [SSL](./docs/Configuration.md#setusesslbool)
- [Batch operation](./docs/BatchExecute.md)
- [Preparation](./docs/Prepare.md)
- [Compression](./docs/Configuration.md#setusecompressionbool)

### Documents

- [Hello World](./docs/HelloWorld.md)
- [Configuration](./docs/Configuration.md)
- [Query](./docs/Query.md)
- [Execute](./docs/Execute.md)
- [Batch Execute](./docs/BatchExecute.md)
- [Prepare](./docs/Prepare.md)
- [Column Types](./docs/ColumnTypes.md)
- [Logging](./docs/Logging.md)
- [Metrics](./docs/Metrics.md)
- [Benchmark](./docs/Benchmark.md)

### Install from ubuntu ppa

Supported version: 18.04 (bionic)

``` text
sudo apt-get install software-properties-common
sudo add-apt-repository ppa:compiv/cpv-project
sudo apt-get update
sudo apt-get install cqldriver
```

### Install from source

Before build the source code of this project you should:

- install [seastar](https://github.com/scylladb/seastar) on system
- ensure `pkg-config --cflags seastar` works
- ensure `pkg-config --libs seastar` works
- ensure the c++ compiler supports c++17

``` text
mkdir -p build/cqldriver-custom
cd build/cqldriver-custom
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ../../src
make V=1
make install V=1
```

### Build and run hello world example

Please check [run.sh](./examples/HelloWorld/run.sh) for the complete build command,
and check the documents for more information.

``` text
cd examples/HelloWorld
sh run.sh
```

### Running tests

Please check [travis_run_tests.sh](./tests/travis_run_tests.sh),
tests are written with google test and require 3 scylla or cassandra instances to work.

### Contribution

You should follow these rules when contributing code, pull request or patch is welcome.

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

