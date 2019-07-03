#!/usr/bin/env bash
BIN_PATH="../../../build/example-benchmark-insert/Main"
mkdir -p $(dirname "${BIN_PATH}")
g++-9 $(pkg-config --cflags seastar) \
	$(pkg-config --cflags cqldriver) \
	Main.cpp \
	$(pkg-config --libs seastar) \
	$(pkg-config --libs cqldriver) \
	-o ${BIN_PATH}
echo "$BIN_PATH --reactor-backend epoll [--prepare] [--compress]"


