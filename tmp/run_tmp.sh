#!/usr/bin/env bash
set -e

BUILDDIR=../build/cqldriver-tmp

mkdir -p ${BUILDDIR}
cd ${BUILDDIR}
cmake -DCMAKE_BUILD_TYPE=Debug \
	-DCMAKE_C_COMPILER=gcc-9 \
	-DCMAKE_CXX_COMPILER=g++-9 \
	../../tmp
make V=1 --jobs=$(printf "%d\n4" $(nproc) | sort -n | head -1)

ASAN_OPTIONS="detect_leaks=1" \
	./CQLDriverTmp --reactor-backend epoll


