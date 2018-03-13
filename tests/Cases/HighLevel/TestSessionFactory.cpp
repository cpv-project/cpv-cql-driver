#include <CQLDriver/HighLevel/SessionFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestSessionFactory, getSession) {
	cql::SessionFactory sessionFactory(
		cql::SessionConfiguration(),
		cql::NodeCollection::create({
			cql::NodeConfiguration()
				.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)
		}));
	for (std::size_t i = 0; i < 3; ++i) {
		auto session = sessionFactory.getSession();
		ASSERT_TRUE(session.isValid());
	}
}

