#pragma once

namespace cql {
	/**
	 * The all in one configuration
	 */
	class CqlSessionConfiguration {
	public:

	private:
		std::vector<TODO> serverAddresses_;
		bool connectWithSsl_;
		std::string username_;
		std::string password_;
		std::size_t maxPoolSize_;
		bool autoDiscoverNodes_;
	};
}

