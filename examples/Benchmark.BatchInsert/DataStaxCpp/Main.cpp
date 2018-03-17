#include <cassandra.h>
#include <iostream>
#include <chrono>

namespace {
	static const std::size_t LoopCount = 1000;
	static const std::size_t InsertCount = 100;

	int createSchema(CassSession* session) {
		CassStatement* dropKeySpaceStatement = cass_statement_new(
			"drop keyspace if exists benchmark_ks", 0);
		CassFuture* dropKeySpaceFuture = cass_session_execute(
			session, dropKeySpaceStatement);
		cass_statement_free(dropKeySpaceStatement);
		CassError error = cass_future_error_code(dropKeySpaceFuture);
		cass_future_free(dropKeySpaceFuture);
		if (error != 0) {
			std::cerr << "drop keyspace error:" << cass_error_desc(error) << std::endl;
			return -1;
		}

		CassStatement* createKeySpaceStatement = cass_statement_new(
			"create keyspace benchmark_ks with replication = "
			"{ 'class': 'SimpleStrategy', 'replication_factor': 1 }", 0);
		CassFuture* createKeySpaceFuture = cass_session_execute(
			session, createKeySpaceStatement);
		cass_statement_free(createKeySpaceStatement);
		error = cass_future_error_code(createKeySpaceFuture);
		cass_future_free(createKeySpaceFuture);
		if (error != 0) {
			std::cerr << "create keyspace error:" << cass_error_desc(error) << std::endl;
			return -1;
		}

		CassStatement* createTableStatement = cass_statement_new(
			"create table benchmark_ks.my_table (id int primary key, name text)", 0);
		CassFuture* createTableFuture = cass_session_execute(
			session, createTableStatement);
		cass_statement_free(createTableStatement);
		error = cass_future_error_code(createTableFuture);
		cass_future_free(createTableFuture);
		if (error != 0) {
			std::cerr << "create table error:" << cass_error_desc(error) << std::endl;
			return -1;
		}
		return 0;
	}

	int insertRecords(CassSession* session) {
		for (std::size_t i = 0; i < LoopCount; ++i) {
			CassBatch* insertBatch = cass_batch_new(CASS_BATCH_TYPE_LOGGED);
			cass_batch_set_consistency(insertBatch, CASS_CONSISTENCY_QUORUM);
			for (std::size_t j = 0; j < InsertCount; ++j) {
				CassStatement* insertStatement = cass_statement_new(
					"insert into benchmark_ks.my_table (id, name) values (?, ?)", 2);
				cass_statement_bind_int32(insertStatement, 0, i*InsertCount+j);
				cass_statement_bind_string(insertStatement, 1, "name");
				cass_batch_add_statement(insertBatch, insertStatement);
				cass_statement_free(insertStatement);
			}
			CassFuture* insertFuture = cass_session_execute_batch(session, insertBatch);
			CassError error = cass_future_error_code(insertFuture);
			cass_future_free(insertFuture);
			if (error != 0) {
				std::cerr << "insert records error:" << cass_error_desc(error) << std::endl;
				return -1;
			}
		}
		return 0;
	}
}

int main() {
	CassCluster* cluster = cass_cluster_new();
	CassSession* session = cass_session_new();
	cass_cluster_set_contact_points(cluster, "127.0.0.1");
	cass_cluster_set_port(cluster, 9043);

	CassFuture* connectFuture = cass_session_connect(session, cluster);
	CassError connectError = cass_future_error_code(connectFuture);
	if (connectError != 0) {
		std::cerr << "connect error:" << cass_error_desc(connectError) << std::endl;
		return -1;
	}

	if (createSchema(session) != 0) {
		return -1;
	}
	auto now = std::chrono::system_clock::now();
	if (insertRecords(session) != 0) {
		return -1;
	}
	auto usedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now() - now).count();
	std::cout << "used seconds: " << usedTime / 1000.0 << std::endl;

	cass_session_free(session);
	cass_cluster_free(cluster);
	return 0;
}

