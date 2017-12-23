#include <core/reactor.hh>
#include "../Utility/CqlObject.hpp"

namespace cql {
	class CqlConnection {
	public:
		CqlConnection(seastar::connected_socket socket) :
			socket_(std::move(socket)) { }
	
	private:
		seastar::connected_socket socket_;
	};
}

