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

