#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from cassandra.cluster import Cluster
from cassandra.query import SimpleStatement
from cassandra import ConsistencyLevel
import sys
import time

LOOP_COUNT=10000
ENABLED_COMPRESSION="--compress" in sys.argv
ENABLED_PREPARATION="--prepare" in sys.argv
DEFAULT_CONSISTENCY=ConsistencyLevel.LOCAL_ONE

if __name__ == "__main__":
	if ENABLED_COMPRESSION:
		print("compression enabled")
	if ENABLED_PREPARATION:
		print("preparation enabled")

	cluster = Cluster(["127.0.0.1"], port=9043, compression=ENABLED_COMPRESSION)
	session = cluster.connect()
	session.default_consistency_level=DEFAULT_CONSISTENCY
	session.execute("drop keyspace if exists benchmark_ks")
	session.execute("create keyspace benchmark_ks with replication = " +
		"{ 'class': 'SimpleStrategy', 'replication_factor': 1 }")
	session.execute("create table benchmark_ks.my_table (id int primary key, name text)")

	start=time.time()
	if ENABLED_PREPARATION:
		insertStatement = session.prepare(
			"insert into benchmark_ks.my_table (id, name) values (?, ?)")
	else:
		insertStatement = SimpleStatement(
			"insert into benchmark_ks.my_table (id, name) values (%s, %s)")
	for i in range(LOOP_COUNT):
		session.execute(insertStatement, (i, "name"))
	print("used seconds: %s"%(time.time()-start))

