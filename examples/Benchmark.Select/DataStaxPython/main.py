#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from cassandra.cluster import Cluster
from cassandra.query import SimpleStatement, BatchStatement
from cassandra import ConsistencyLevel
import time

LOOP_COUNT=10000
SELECT_COUNT=100

if __name__ == "__main__":
	cluster = Cluster(["127.0.0.1"], port=9043)
	session = cluster.connect()
	session.execute("drop keyspace if exists benchmark_ks")
	session.execute("create keyspace benchmark_ks with replication = " +
		"{ 'class': 'SimpleStrategy', 'replication_factor': 1 }")
	session.execute("create table benchmark_ks.my_table (id int primary key, name text)")

	insertStatement = SimpleStatement(
		"insert into benchmark_ks.my_table (id, name) values (%s, %s)",
		consistency_level=ConsistencyLevel.QUORUM)
	insertBatch = BatchStatement()
	for i in range(SELECT_COUNT*2):
		insertBatch.add(insertStatement, (i, "name"))
	session.execute(insertBatch)

	start=time.time()
	selectStatement = SimpleStatement(
		"select id, name from benchmark_ks.my_table limit %d"%SELECT_COUNT,
		consistency_level=ConsistencyLevel.QUORUM)
	for i in range(LOOP_COUNT):
		rows = session.execute(selectStatement)
		for row in rows:
			id, name = row.id, row.name
			# print(id, name)
	print("used seconds:", time.time()-start)

