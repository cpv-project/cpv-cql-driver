#!/usr/bin/env bash
BIN_PATH="../../../bin/benchmark"
set -e
g++-9 -O3 Main.cpp -lcassandra -lpthread -o $BIN_PATH
echo "$BIN_PATH [--prepare] [--compress]"

