#include <iostream>
#include <core/app-template.hh>
#include <CQLDriver/CQLDriver.hpp>

int main(int argc, char** argv) {
	using namespace cql;
	seastar::app_template app;
	app.run(argc, argv, [] {
		SessionFactory sessionFactory(
			SessionConfiguration()
				.setDefaultKeySpace("system"),
			NodeCollection::create({
				NodeConfiguration()
					.setAddress("127.0.0.1", 9043)
			}));
		auto session = sessionFactory.getSession();
		return session.query(Command("select * from batchlog")).then([] (auto result) {
			std::cout << "columns count: " << result.getColumnsCount() << std::endl;
			std::cout << "rows count: " << result.getRowsCount() << std::endl;
		});
	});
	return 0;
}

