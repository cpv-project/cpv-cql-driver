#include <iostream>
#include <seastar/core/app-template.hh>
#include <CQLDriver/CQLDriver.hpp>

int main(int argc, char** argv) {
	seastar::app_template app;
	app.run(argc, argv, [] {
		cql::SessionFactory sessionFactory(
			cql::SessionConfiguration()
				.setDefaultKeySpace("system"),
			cql::NodeCollection::create({
				cql::NodeConfiguration()
					.setAddress("127.0.0.1", 9043)
			}));
		cql::Session session = sessionFactory.getSession();
		cql::Command command("select * from batchlog");
		return session.query(std::move(command)).then([] (cql::ResultSet result) {
			std::cout << "columns count: " << result.getColumnsCount() << std::endl;
			std::cout << "rows count: " << result.getRowsCount() << std::endl;
		});
	});
	return 0;
}

