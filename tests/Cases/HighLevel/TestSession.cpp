#include <core/do_with.hh>
#include <CQLDriver/Common/ColumnTypes/Int.hpp>
#include <CQLDriver/Common/ColumnTypes/Text.hpp>
#include <CQLDriver/Common/Exceptions/ResponseErrorException.hpp>
#include <CQLDriver/HighLevel/SessionFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST_FUTURE(TestSession, queryInSystem) {
	cql::SessionFactory sessionFactory(
		cql::SessionConfiguration()
			.setDefaultKeySpace("system"),
		cql::NodeCollection::create({
			cql::NodeConfiguration()
				.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)
		}));
	auto session = sessionFactory.getSession();
	return session.query(cql::Command("select * from batchlog")).then([] (auto result) {
		ASSERT_TRUE(result.isValid());
	});
}

TEST_FUTURE(TestSession, queryError) {
	cql::SessionFactory sessionFactory(
		cql::SessionConfiguration()
			.setDefaultKeySpace("system"),
		cql::NodeCollection::create({
			cql::NodeConfiguration()
				.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)
		}));
	auto session = sessionFactory.getSession();
	return session.query(cql::Command("select * from notexisttable")).then_wrapped([] (auto&& f) {
		ASSERT_THROWS_CONTAINS(
			cql::ResponseErrorException, f.get(),
			"InvalidQuery: unconfigured table notexisttable");
	});
}

TEST_FUTURE(TestSession, execute) {
	cql::SessionFactory sessionFactory(
		cql::SessionConfiguration(),
		cql::NodeCollection::create({
			cql::NodeConfiguration()
				.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)
		}));
	auto session = sessionFactory.getSession();
	return seastar::do_with(std::move(session), [] (auto& session) {
		return seastar::make_ready_future<>().then([&session] {
			return session.execute(cql::Command("drop keyspace if exists testkeyspace"));
		}).then([&session] {
			return session.execute(cql::Command(
				"create keyspace testkeyspace with replication = "
				"{ 'class': 'SimpleStrategy', 'replication_factor': 1 }"));
		}).then([&session] {
			return session.execute(cql::Command(
				"create table testkeyspace.testtable (id int primary key, name text)"));
		}).then([&session] {
			return session.execute(cql::Command(
				"insert into testkeyspace.testtable (id, name) values (?,?)")
				.addParameters(cql::Int(1), cql::Text("abc")));
		}).then([&session] {
			return session.query(cql::Command(
				"select id, name from testkeyspace.testtable"));
		}).then([] (auto result) {
			ASSERT_EQ(result.getRowsCount(), 1);
			ASSERT_EQ(result.getColumnsCount(), 2);
			cql::Int id;
			cql::Text name;
			result.fill(id, name);
			ASSERT_EQ(id, 1);
			ASSERT_EQ(name, "abc");
			ASSERT_EQ(result.getDecodePtr(), result.getDecodeEnd());
		});
	});
}

TEST_FUTURE(TestSession, executeError) {
	cql::SessionFactory sessionFactory(
		cql::SessionConfiguration(),
		cql::NodeCollection::create({
			cql::NodeConfiguration()
				.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)
		}));
	auto session = sessionFactory.getSession();
	return session.execute(cql::Command("drop keyspace notexistkeyspace")).then_wrapped([] (auto&& f) {
		ASSERT_THROWS_CONTAINS(
			cql::ResponseErrorException, f.get(),
			"Cannot drop non existing keyspace 'notexistkeyspace'");
	});
}

TEST_FUTURE(TestSession, batchExecute) {
	cql::SessionFactory sessionFactory(
		cql::SessionConfiguration(),
		cql::NodeCollection::create({
			cql::NodeConfiguration()
				.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)
		}));
	auto session = sessionFactory.getSession();
	return seastar::do_with(std::move(session), [] (auto& session) {
		return seastar::make_ready_future<>().then([&session] {
			return session.execute(cql::Command("drop keyspace if exists testkeyspace"));
		}).then([&session] {
			return session.execute(cql::Command(
				"create keyspace testkeyspace with replication = "
				"{ 'class': 'SimpleStrategy', 'replication_factor': 1 }"));
		}).then([&session] {
			return session.execute(cql::Command(
				"create table testkeyspace.testtable (id int primary key, name text)"));
		}).then([&session] {
			return session.batchExecute(cql::BatchCommand()
				.addQuery("insert into testkeyspace.testtable (id, name) values (?,?)")
					.prepareQuery()
					.openParameterSet()
						.addParameters(cql::Int(1), cql::Text("a"))
					.openParameterSet()
						.addParameters(cql::Int(2), cql::Text("b"))
					.openParameterSet()
						.addParameters(cql::Int(3), cql::Text("c"))
				.addQuery("update testkeyspace.testtable set name = ? where id = ?")
					.addParameters(cql::Text("aaa"), cql::Int(1)));
		}).then([&session] {
			return session.query(cql::Command(
				"select id, name from testkeyspace.testtable"));
		}).then([] (auto result) {
			ASSERT_EQ(result.getRowsCount(), 3);
			ASSERT_EQ(result.getColumnsCount(), 2);
			cql::Int id;
			cql::Text name;
			result.fill(id, name);
			ASSERT_EQ(id, 1);
			ASSERT_EQ(name, "aaa");
			result.fill(id, name);
			ASSERT_EQ(id, 2);
			ASSERT_EQ(name, "b");
			result.fill(id, name);
			ASSERT_EQ(id, 3);
			ASSERT_EQ(name, "c");
			ASSERT_EQ(result.getDecodePtr(), result.getDecodeEnd());
		});
	});
}

TEST_FUTURE(TestSession, batchExecuteError) {
	cql::SessionFactory sessionFactory(
		cql::SessionConfiguration(),
		cql::NodeCollection::create({
			cql::NodeConfiguration()
				.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)
		}));
	auto session = sessionFactory.getSession();
	return seastar::do_with(std::move(session), [] (auto& session) {
		return seastar::make_ready_future<>().then([&session] {
			return session.execute(cql::Command("drop keyspace if exists testkeyspace"));
		}).then([&session] {
			return session.batchExecute(cql::BatchCommand()
				.addQuery("insert into testkeyspace.testtable (id, name) values (?,?)")
					.prepareQuery()
					.openParameterSet()
						.addParameters(cql::Int(1), cql::Text("a")));
		}).then_wrapped([] (auto&& f) {
			ASSERT_THROWS_CONTAINS(
				cql::ResponseErrorException, f.get(),
				"Keyspace testkeyspace does not exist");
		});
	});
}

