#!/usr/bin/env bash
PKGCONFIG_PATH="../../../bin/release/cqldriver.pc"
BIN_PATH="../../../bin/benchmark"
g++ $(pkg-config --cflags $PKGCONFIG_PATH) Main.cpp $(pkg-config --libs $PKGCONFIG_PATH) -o $BIN_PATH && $BIN_PATH

