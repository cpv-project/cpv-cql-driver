#include <seastar/core/reactor.hh>
#include <CQLDriver/Common/Logger.hpp>
#include "./Loggers/ConsoleLogger.hpp"
#include "./Loggers/NoopLogger.hpp"

namespace cql {
	/** Constructor */
	Logger::Logger(LogLevel logLevel) :
		logLevel_(logLevel) { }

	/** Create a console logger */
	seastar::shared_ptr<Logger> Logger::createConsole(LogLevel logLevel) {
		return seastar::make_shared<ConsoleLogger>(logLevel);
	}

	/** Create a noop logger */
	seastar::shared_ptr<Logger> Logger::createNoop() {
		return seastar::make_shared<NoopLogger>();
	}

	/** Get thread id for logging */
	std::size_t Logger::getThreadId() {
		return seastar::engine().cpu_id();
	}
}

