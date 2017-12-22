#!/usr/bin/env bash
# break after command error
set -e

# build seastar debug
# use single core because it require so much memory (require atleast 3.5G for per core)
cd 3rd-party/seastar
./configure.py --mode=release --enable-dpdk
ninja -j1
cd ..

# build cql driver debug
mkdir -p bin/release
cd bin/release
cmake -DCMAKE_BUILD_TYPE=Release ../../src
make
cd ..

