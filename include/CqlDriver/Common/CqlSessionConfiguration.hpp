#pragma once
#include <cstdint>
#include <chrono>

namespace cql {
	/**
	 * The configuration of all sessions
	 */
	class CqlSessionConfiguration {
	public:
		/**
		 * Set the maximum database connection pool size.
		 * Notice it's per core configuration,
		 * for example if the max pool size is 100 and there 2 cpu cores,
		 * then the maximum database connections in the pool for single process is 200.
		 * The default value is 100.
		 */
		CqlSessionConfiguration& setMaxPoolSize(std::size_t maxPoolSize);

		/**
		 * Set the dns cache time for the hostname of nodes.
		 * The default value is 30000ms.
		 */
		CqlSessionConfiguration& setDnsCacheTime(std::chrono::milliseconds dnsCacheTime);

		/** Get the maximum database connection pool size */
		std::size_t getMaxPoolSize() const;

		/** Get the dns cache time for the hostname of nodes */
		std::chrono::milliseconds getDnsCacheTime() const;

		/** Constructor */
		CqlSessionConfiguration();

	private:
		std::size_t maxPoolSize_;
		std::chrono::milliseconds dnsCacheTime_;
	};
}

