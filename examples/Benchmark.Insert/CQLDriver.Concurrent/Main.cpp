#include <cstring>
#include <iostream>
#include <chrono>
#include <atomic>
#include <seastar/core/do_with.hh>
#include <seastar/core/future-util.hh>
#include <seastar/core/app-template.hh>
#include <seastar/core/semaphore.hh>
#include <seastar/core/gate.hh>
#include <seastar/core/reactor.hh>
#include <boost/range/irange.hpp>
#include <boost/program_options.hpp>
#include <CQLDriver/CQLDriver.hpp>

namespace {
	static const std::size_t LoopCount = 100000;
	static const std::size_t ParallelDegreePerCore = 20;
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
					.setMaxStreams(ParallelDegreePerCore+1)
			}));
		return sessionFactory;
	}

	seastar::future<> initDatabase() {
		auto sessionFactory = makeSessionFactory();
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

	seastar::future<> benchmark(std::atomic_size_t& loopCount) {
		auto sessionFactory = makeSessionFactory();
		cql::Session session = sessionFactory.getSession();
		return seastar::do_with(
			seastar::semaphore(ParallelDegreePerCore),
			seastar::gate(),
			sessionFactory,
			[&loopCount] (auto& limit, auto& gate, auto& sessionFactory) {
			return seastar::repeat([&loopCount, &limit, &gate, &sessionFactory] {
				std::size_t id = loopCount.fetch_add(1);
				if (id >= LoopCount) {
					return seastar::make_ready_future<seastar::stop_iteration>(
						seastar::stop_iteration::yes);
				}
				return seastar::get_units(limit, 1).then(
					[&gate, &sessionFactory, id] (auto units) {
					gate.enter();
					auto session = sessionFactory.getSession();
					auto command = cql::Command("insert into benchmark_ks.my_table (id, name) values (?, ?)")
						.addParameters(cql::Int(id), cql::MemRef("name"));
					session.execute(std::move(command))
					.finally([&gate, units = std::move(units)] {
						gate.leave();
					});
					return seastar::stop_iteration::no;
				});
			}).finally([&gate] {
				return gate.close();
			});
		});
	}
}

int main(int argc, char** argv) {
	namespace bpo = boost::program_options;
	seastar::app_template app;
	app.add_options()("prepare", "enabled preparation");
	app.add_options()("compress", "enabled compression");

	std::atomic_size_t loopCount(0);
	app.run(argc, argv, [&app, &loopCount] {
		auto& config = app.configuration();
		EnablePreparation = config.count("prepare") > 0;
		EnableCompression = config.count("compress") > 0;
		if (EnablePreparation) {
			std::cout << "preparation enabled" << std::endl;
		}
		if (EnableCompression) {
			std::cout << "compression enabled" << std::endl;
		}

		return initDatabase().then([&loopCount] {
			auto start = std::chrono::system_clock::now();
			std::cout << "smp count: " << seastar::smp::count << std::endl;
			return seastar::parallel_for_each(
				boost::irange<std::size_t>(0, seastar::smp::count),
				[&loopCount] (std::size_t core) {
				return seastar::smp::submit_to(core, [&loopCount] {
					return benchmark(loopCount);
				});
			}).then([start] {
				auto usedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
					std::chrono::system_clock::now() - start).count();
				std::cout << "used seconds: " << usedTime / 1000.0 << std::endl;
			});
		});
	});
	return 0;
}

