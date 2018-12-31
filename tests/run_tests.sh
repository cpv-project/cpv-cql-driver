#!/usr/bin/env bash
set -e

BUILDDIR=../build/cqldriver-tests

mkdir -p ${BUILDDIR}
cd ${BUILDDIR}
cmake -DCMAKE_BUILD_TYPE=Debug \
	-DDB_SIMPLE_IP=172.17.0.1 \
	-DDB_SIMPLE_PORT=9043 \
	-DDB_SSL_1_IP=172.17.0.1 \
	-DDB_SSL_1_PORT=9044 \
	-DDB_SSL_2_IP=172.17.0.1 \
	-DDB_SSL_2_PORT=9045 \
	-DGTEST_SOURCE_DIR=/usr/src/gtest \
	../../tests
make V=1

ASAN_OPTIONS="detect_leaks=1" \
	./CQLDriverTests --reactor-backend epoll


