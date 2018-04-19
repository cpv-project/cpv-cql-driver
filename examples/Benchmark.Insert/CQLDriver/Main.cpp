#include <iostream>
#include <chrono>
#include <core/do_with.hh>
#include <core/future-util.hh>
#include <core/app-template.hh>
#include <boost/program_options.hpp>
#include <CQLDriver/CQLDriver.hpp>

namespace {
	static const std::size_t LoopCount = 10000;
	static bool EnableCompression = false;
	static bool EnablePreparation = false;
	static cql::ConsistencyLevel DefaultConsistencyLevel = cql::ConsistencyLevel::LocalOne;

	cql::SessionFactory makeSessionFactory() {
		cql::SessionFactory sessionFactory(
			cql::SessionConfiguration()
				.setDefaultConsistency(DefaultConsistencyLevel)
				.setPrepareAllQueries(EnablePreparation)
				.setMinPoolSize(1),
			cql::NodeCollection::create({
				cql::NodeConfiguration()
					.setAddress("127.0.0.1", 9043)
					.setUseCompression(EnableCompression)
			}));
		return sessionFactory;
	}

	seastar::future<> initDatabase(cql::SessionFactory sessionFactory) {
		cql::Session session = sessionFactory.getSession();
		return seastar::do_with(std::move(session), [] (auto& session) {
			return session.execute(cql::Command(
					"drop keyspace if exists benchmark_ks"))
			.then([&session] {
				return session.execute(cql::Command(
					"create keyspace benchmark_ks with replication = "
					"{ 'class': 'SimpleStrategy', 'replication_factor': 1 }"));
			}).then([&session] {
				return session.execute(cql::Command(
					"create table benchmark_ks.my_table (id int primary key, name text)"));
			});
		});
	}

	seastar::future<> benchmark(cql::SessionFactory sessionFactory) {
		cql::Session session = sessionFactory.getSession();
		return seastar::do_with(
			std::move(session),
			static_cast<size_t>(0),
			[] (auto& session, auto& loopCount) {
			return seastar::repeat([&session, &loopCount] {
				auto command = cql::Command("insert into benchmark_ks.my_table (id, name) values (?, ?)")
					.addParameters(cql::Int(loopCount), cql::MemRef("name"));
				return session.execute(std::move(command)).then([&loopCount] {
					loopCount += 1;
					return loopCount < LoopCount ?
						seastar::stop_iteration::no :
						seastar::stop_iteration::yes;
				});
			});
		});
	}
}

int main(int argc, char** argv) {
	namespace bpo = boost::program_options;
	seastar::app_template app;
	app.add_options()("prepare", "enabled preparation");
	app.add_options()("compress", "enabled compression");

	app.run(argc, argv, [&app] {
		auto& config = app.configuration();
		EnablePreparation = config.count("prepare") > 0;
		EnableCompression = config.count("compress") > 0;
		if (EnablePreparation) {
			std::cout << "preparation enabled" << std::endl;
		}
		if (EnableCompression) {
			std::cout << "compression enabled" << std::endl;
		}

		auto sessionFactory = makeSessionFactory();
		return initDatabase(sessionFactory).then([sessionFactory] {
			auto start = std::chrono::system_clock::now();
			return benchmark(sessionFactory).then([start] {
				auto usedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
					std::chrono::system_clock::now() - start).count();
				std::cout << "used seconds: " << usedTime / 1000.0 << std::endl;
			});
		});
	});
	return 0;
}

