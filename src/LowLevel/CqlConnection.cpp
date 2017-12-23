#include <core/reactor.hh>

namespace cql {
	class CqlConnection {
	public:
		CqlConnection(seastar::connected_socket socket) :
			socket_(std::move(socket)) { }
	
	private:
		seastar::connected_socket socket_;
	};
}

