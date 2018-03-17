#include <iostream>
#include <chrono>
#include <core/do_with.hh>
#include <core/future-util.hh>
#include <core/app-template.hh>
#include <CQLDriver/CQLDriver.hpp>

namespace {
	static const std::size_t LoopCount = 10000;
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
			}).then([&session, &start, &loopCount] {
				start = std::chrono::system_clock::now();
				loopCount = 0;
				return seastar::repeat([&session, &loopCount] {
					auto command = cql::Command("insert into benchmark_ks.my_table (id, name) values (?, ?)")
						.setConsistency(cql::ConsistencyLevel::Quorum)
						.addParameters(cql::Int(loopCount), cql::MemRef("name"));
					return session.execute(std::move(command)).then([&loopCount] {
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
