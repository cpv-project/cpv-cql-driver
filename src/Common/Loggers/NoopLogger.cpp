#include "./NoopLogger.hpp"

namespace cql {
	/** Constructor */
	NoopLogger::NoopLogger() :
		Logger(LogLevel::Emergency) { }

	/** The implmentation of log, do nothing */
	void NoopLogger::logImpl(LogLevel, const std::string&) { }
}

