#include <cassandra.h>
#include <cstring>
#include <iostream>
#include <chrono>

namespace {
	static const std::size_t LoopCount = 10000;
	static bool EnableCompression = false;
	static bool EnablePreparation = false;
	static CassConsistency DefaultConsistencyLevel = CASS_CONSISTENCY_LOCAL_ONE;

	void parseArguments(int argc, char** argv) {
		for (int i = 1; i < argc; ++i) {
			const char* arg = argv[i];
			if (std::strcmp(arg, "--prepare") == 0) {
				EnablePreparation = true;
				std::cout << "preparation enabled" << std::endl;
			} else if (std::strcmp(arg, "--compress") == 0) {
				EnableCompression = true;
				std::cout << "compression is unsupported yet" << std::endl;
				exit(1);
			}
		}
	}

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
		const char* insertQuery = "insert into benchmark_ks.my_table (id, name) values (?, ?)";
		const CassPrepared* prepared = nullptr;
		if (EnablePreparation) {
			CassFuture* prepareFuture = cass_session_prepare(session, insertQuery);
			CassError prepareError = cass_future_error_code(prepareFuture);
			if (prepareError != 0) {
				std::cerr << "prepare statement error:" << cass_error_desc(prepareError) << std::endl;
				cass_future_free(prepareFuture);
				return -1;
			}
			prepared = cass_future_get_prepared(prepareFuture);
			cass_future_free(prepareFuture);
		}
		int ret = 0;
		for (std::size_t i = 0, j = LoopCount; i < j; ++i) {
			CassStatement* insertStatement = nullptr;
			if (EnablePreparation) {
				insertStatement = cass_prepared_bind(prepared);
			} else {
				insertStatement = cass_statement_new(insertQuery, 2);
			}
			cass_statement_bind_int32(insertStatement, 0, i);
			cass_statement_bind_string(insertStatement, 1, "name");
			CassFuture* insertFuture = cass_session_execute(session, insertStatement);
			cass_statement_free(insertStatement);
			CassError error = cass_future_error_code(insertFuture);
			cass_future_free(insertFuture);
			if (error != 0) {
				std::cerr << "insert records error:" << cass_error_desc(error) << std::endl;
				ret = -1;
				break;
			}
		}
		if (prepared != nullptr) {
			cass_prepared_free(prepared);
		}
		return ret;
	}
}

int main(int argc, char** argv) {
	parseArguments(argc, argv);

	CassCluster* cluster = cass_cluster_new();
	CassSession* session = cass_session_new();
	cass_cluster_set_contact_points(cluster, "127.0.0.1");
	cass_cluster_set_port(cluster, 9043);
	cass_cluster_set_consistency(cluster, DefaultConsistencyLevel);

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

