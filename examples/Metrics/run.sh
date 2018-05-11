#!/usr/bin/env bash
PKGCONFIG_PATH="../../bin/debug/cqldriver.pc"
BIN_PATH="../../bin/example"
g++ $(pkg-config --cflags $PKGCONFIG_PATH) Main.cpp $(pkg-config --libs $PKGCONFIG_PATH) -o $BIN_PATH && \
	echo "visit http://127.0.0.1:8000/metrics" && \
	$BIN_PATH

