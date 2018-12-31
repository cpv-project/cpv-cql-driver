#include <iostream>
#include <seastar/core/do_with.hh>
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
		return seastar::do_with(std::move(session), [] (auto& session) {
			return seastar::make_ready_future<>().then([&session] {
				return session.execute(cql::Command(
					"drop keyspace if exists exmple_ks"));
			}).then([&session] {
				return session.execute(cql::Command(
					"create keyspace exmple_ks with replication = "
					"{ 'class': 'SimpleStrategy', 'replication_factor': 1 }"));
			}).then([&session] {
				return session.execute(cql::Command(
					"create table exmple_ks.my_table (id int primary key, name text)"));
			}).then([&session] {
				return session.batchExecute(cql::BatchCommand()
					.addQuery("insert into exmple_ks.my_table (id, name) values (?,?)")
						.prepareQuery()
						.openParameterSet()
							.addParameters(cql::Int(1), cql::Text("a"))
						.openParameterSet()
							.addParameters(cql::Int(2), cql::Text("b"))
						.openParameterSet()
							.addParameters(cql::Int(3), cql::Text("c"))
					.addQuery("update exmple_ks.my_table set name = ? where id = ?")
						.addParameters(cql::Text("aaa"), cql::Int(1))
					.addQuery("delete from exmple_ks.my_table where id = ?")
						.addParameters(cql::Int(3)));
			}).then([&session] {
				return session.query(cql::Command(
					"select id, name from exmple_ks.my_table"));
			}).then([] (cql::ResultSet result) {
				std::cout << "columns count: " << result.getColumnsCount() << std::endl;
				std::cout << "rows count: " << result.getRowsCount() << std::endl;
				cql::Int id;
				cql::Text name;
				for (std::size_t i = 0, j = result.getRowsCount(); i < j; ++i) {
					result.fill(id, name);
					std::cout << "<record> id: " << id << " name: " << name << std::endl;
				}
			});
		});
	});
	return 0;
}

