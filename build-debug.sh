#!/usr/bin/env bash
# break after command error
set -e

# build seastar
# use single core because it require so much memory (require atleast 3.5G for per core)
cd 3rd-party/seastar
if [ ! -f build.ninja ]; then
	./configure.py --mode=all --with libseastar.a --with seastar.pc --enable-dpdk
fi
ninja -j1
cd ../..

# build cql driver
mkdir -p bin/debug
cd bin/debug
cmake -DCMAKE_BUILD_TYPE=Debug ../../src
make V=1
cd ../..

