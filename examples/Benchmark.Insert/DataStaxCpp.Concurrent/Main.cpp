#include <sys/sysinfo.h>
#include <cassandra.h>
#include <cstring>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <atomic>

namespace {
	static const std::size_t LoopCount = 100000;
	static const std::size_t ParallelDegreePerCore = 20;
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

	CassStatement* getInsertStatement(CassSession* session) {
		const char* insertQuery = "insert into benchmark_ks.my_table (id, name) values (?, ?)";
		static thread_local const CassPrepared* prepared = nullptr;
		if (EnablePreparation && prepared == nullptr) {
			CassFuture* prepareFuture = cass_session_prepare(session, insertQuery);
			CassError prepareError = cass_future_error_code(prepareFuture);
			if (prepareError != 0) {
				std::cerr << "prepare statement error:" << cass_error_desc(prepareError) << std::endl;
				cass_future_free(prepareFuture);
			} else {
				prepared = cass_future_get_prepared(prepareFuture);
				cass_future_free(prepareFuture);
				return cass_prepared_bind(prepared);
			}
		}
		return cass_statement_new(insertQuery, 2);
	}

	int insertRecords(CassCluster* cluster, std::atomic_size_t& loopCount) {
		CassSession* session = cass_session_new();
		CassFuture* connectFuture = cass_session_connect(session, cluster);
		CassError connectError = cass_future_error_code(connectFuture);
		cass_future_free(connectFuture);
		if (connectError != 0) {
			std::cerr << "connect error:" << cass_error_desc(connectError) << std::endl;
			return -1;
		}

		std::atomic_int workingCount(0);
		while (true) {
			std::size_t i = loopCount.fetch_add(1);
			if (i >= LoopCount) {
				break;
			}
			workingCount.fetch_add(1);
			CassStatement* insertStatement = getInsertStatement(session);
			cass_statement_bind_int32(insertStatement, 0, i);
			cass_statement_bind_string(insertStatement, 1, "name");
			CassFuture* insertFuture = cass_session_execute(session, insertStatement);
			cass_statement_free(insertStatement);
			CassError callbackError = cass_future_set_callback(insertFuture, [](auto future, auto data) {
				auto& workingCount = *reinterpret_cast<std::atomic_size_t*>(data);
				CassError error = cass_future_error_code(future);
				if (error != 0) {
					std::cerr << "insert records error:" << cass_error_desc(error) << std::endl;
				}
				workingCount.fetch_add(-1);
			}, &workingCount);
			cass_future_free(insertFuture);
			if (callbackError != 0) {
				std::cerr << "set callback error:" << cass_error_desc(callbackError) << std::endl;
				return -1;
			}
			while (workingCount > ParallelDegreePerCore) {
				std::this_thread::sleep_for(std::chrono::microseconds(100));
			}
		}
		while (workingCount > 0) {
			std::this_thread::sleep_for(std::chrono::microseconds(100));
		}
		return 0;
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
	std::vector<std::thread> threads;
	std::atomic_size_t loopCount(0);
	for (std::size_t i = 0, j = get_nprocs(); i < j; ++i) {
		threads.emplace_back([&cluster, &loopCount] {
			insertRecords(cluster, loopCount);
		});
	}
	for (auto& thread : threads) {
		thread.join();
	}
	auto usedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now() - now).count();
	std::cout << "used seconds: " << usedTime / 1000.0 << std::endl;

	cass_session_free(session);
	cass_cluster_free(cluster);
	return 0;
}

