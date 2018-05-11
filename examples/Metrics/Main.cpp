#include <iostream>
#include <atomic>
#include <core/app-template.hh>
#include <core/sleep.hh>
#include <core/prometheus.hh>
#include <CQLDriver/CQLDriver.hpp>

namespace {
	std::atomic_bool Running = true;

	void sigintHandler(int) {
		std::cout << "exit application..." << std::endl;
		Running = false;
	}

	seastar::future<> action(cql::SessionFactory& sessionFactory) {
		cql::Session session = sessionFactory.getSession();
		cql::Command command("select * from batchlog");
		return session.query(std::move(command)).then([] (cql::ResultSet result) {
			std::cout << "columns count: " << result.getColumnsCount() << std::endl;
			std::cout << "rows count: " << result.getRowsCount() << std::endl;
		});
	}
}

int main(int argc, char** argv) {
	seastar::app_template app;
	app.run(argc, argv, [] {
		signal(SIGINT, sigintHandler);
		cql::SessionFactory sessionFactory(
			cql::SessionConfiguration()
				.setDefaultKeySpace("system")
				.setLogger(cql::Logger::createConsole(cql::LogLevel::Debug)),
			cql::NodeCollection::create({
				cql::NodeConfiguration()
					.setAddress("127.0.0.1", 9043)
			}));
		auto server = seastar::make_shared<seastar::httpd::http_server_control>();
		return seastar::do_with(
			std::move(sessionFactory),
			std::move(server),
			[] (auto& sessionFactory, auto& server) {
			return server->start().then([&server] {
				seastar::prometheus::config ctx;
				return seastar::prometheus::start(*server, ctx);
			}).then([&server] {
				return server->listen({ "127.0.0.1", 8000 });
			}).then([&sessionFactory] {
				return seastar::repeat([&sessionFactory] {
					return seastar::sleep(std::chrono::seconds(1)).then([&sessionFactory] {
						return action(sessionFactory);
					}).then([] {
						return Running ?
							seastar::stop_iteration::no :
							seastar::stop_iteration::yes;
					});
				});
			}).finally([&server] {
				return server->stop();
			});
		});
	});
	return 0;
}

