#include <seastar/core/reactor.hh>
#include <CQLDriver/Common/Utility/SocketHolder.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST_FUTURE(TestSocketHolder, all) {
	seastar::socket_address address(seastar::ipv4_addr(DB_SIMPLE_IP, DB_SIMPLE_PORT));
	return seastar::engine().net().connect(address).then([] (seastar::connected_socket&& socket) {
		cql::SocketHolder holder;
		ASSERT_FALSE(holder.isConnected());
		holder = cql::SocketHolder(std::move(socket));
		ASSERT_TRUE(holder.isConnected());
	});
}

