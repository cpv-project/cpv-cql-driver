#!/usr/bin/env bash
PKGCONFIG_PATH="../../../bin/release/cqldriver.pc"
BIN_PATH="../../../bin/benchmark"
set -e
g++ $(pkg-config --cflags $PKGCONFIG_PATH) Main.cpp $(pkg-config --libs $PKGCONFIG_PATH) -o $BIN_PATH
echo "$BIN_PATH [--prepare] [--compress]"

