#include <iostream>
#include <chrono>
#include <core/do_with.hh>
#include <core/future-util.hh>
#include <core/app-template.hh>
#include <CQLDriver/CQLDriver.hpp>

namespace {
	static const std::size_t LoopCount = 10000;
	static const std::size_t SelectCount = 100;
}

int main(int argc, char** argv) {
	seastar::app_template app;
	app.run(argc, argv, [] {
		cql::SessionFactory sessionFactory(
			cql::SessionConfiguration()
				.setMinPoolSize(1),
			cql::NodeCollection::create({
				cql::NodeConfiguration()
					.setAddress("127.0.0.1", 9043)
			}));
		cql::Session session = sessionFactory.getSession();
		return seastar::do_with(
			std::move(session),
			std::chrono::system_clock::time_point(),
			static_cast<size_t>(0),
			[] (auto& session, auto& start, auto& loopCount) {
			return seastar::make_ready_future<>().then([&session] {
				return session.execute(cql::Command(
					"drop keyspace if exists benchmark_ks"));
			}).then([&session] {
				return session.execute(cql::Command(
					"create keyspace benchmark_ks with replication = "
					"{ 'class': 'SimpleStrategy', 'replication_factor': 1 }"));
			}).then([&session] {
				return session.execute(cql::Command(
					"create table benchmark_ks.my_table (id int primary key, name text)"));
			}).then([&session] {
				cql::BatchCommand command;
				command.setConsistency(cql::ConsistencyLevel::Quorum);
				command.addQuery("insert into benchmark_ks.my_table (id, name) values (?, ?)");
				cql::Int id;
				cql::MemRef name("name");
				for (std::size_t i = 0, j = SelectCount*2; i < j; ++i) {
					id = i;
					command.openParameterSet();
					command.addParameters(id, name);
				}
				return session.batchExecute(std::move(command));
			}).then([&session, &start, &loopCount] {
				start = std::chrono::system_clock::now();
				loopCount = 0;
				return seastar::repeat([&session, &loopCount] {
					auto command = cql::Command("select id, name from benchmark_ks.my_table")
						.setConsistency(cql::ConsistencyLevel::Quorum)
						.setPageSize(SelectCount);
					return session.query(std::move(command))
					.then([&loopCount] (auto result) {
						cql::Int id;
						cql::MemRef name;
						for (std::size_t i = 0, j = result.getRowsCount(); i < j; ++i) {
							result.fill(id, name);
							// std::cout << id << " " << name << std::endl;
						}
						loopCount += 1;
						return loopCount < LoopCount ?
							seastar::stop_iteration::no :
							seastar::stop_iteration::yes;
					});
				}).then([&start] {
					auto usedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
						std::chrono::system_clock::now() - start).count();
					std::cout << "used seconds: " << usedTime / 1000.0 << std::endl;
				});
			});
		});
	});
	return 0;
}
