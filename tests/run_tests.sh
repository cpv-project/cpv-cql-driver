#!/usr/bin/env bash
set -e

BUILDDIR=../build/cqldriver-tests

mkdir -p ${BUILDDIR}
cd ${BUILDDIR}
cmake -DCMAKE_BUILD_TYPE=Debug \
	-DDB_SIMPLE_IP=127.0.0.1 \
	-DDB_SIMPLE_PORT=9043 \
	-DDB_SSL_1_IP=localhost \
	-DDB_SSL_1_PORT=9044 \
	-DDB_SSL_2_IP=localhost \
	-DDB_SSL_2_PORT=9045 \
	-DGTEST_SOURCE_DIR=/usr/src/gtest \
	-DCQL_ENABLE_DEBUG_LOGGER_BY_DEFAULT=ON \
	../../tests
make V=1 --jobs=$(printf "%d\n4" $(nproc) | sort -n | head -1)

ASAN_OPTIONS="detect_leaks=1" \
	./CQLDriverTests --reactor-backend epoll


