#!/usr/bin/env bash
BIN_PACH="../../../bin/benchmark"
g++ -O3 Main.cpp -lcassandra -o $BIN_PACH && $BIN_PACH


