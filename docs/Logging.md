# Logging

`Logger` can be used to capture internal logs to find and solve problems.<br/>
This library contains two default `Logger` implementions:

## ConsoleLogger

A logger writes messages to console.<br/>
The example code to create it:<br/>

``` c++
auto logger = cql::Logger::createConsole(cql::LogLevel::Debug);
```

Supported log levels:

- LogLevel::Emergency
- LogLevel::Alert
- LogLevel::Critical
- LogLevel::Error
- LogLevel::Warning
- LogLevel::Notice
- LogLevel::Info
- LogLevel::Debug

## NoopLogger

A do-nothing logger.

``` c++
auto logger = cql::Logger::createNoop();
```

## Use logger

You can specify the logger in session configuration by calling `SessionConfiguration::setLogger`, for example:

``` c++
auto configuration = cql::SessionConfiguration()
	.setMinPoolSize(1)
	.setMaxPoolSize(100)
	.setDefaultKeySpace("system")
	.setLogger(cql::Logger::createConsole(cql::LogLevel::Debug));
```

