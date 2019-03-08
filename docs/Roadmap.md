# Roadmap

## 0.6

Changes:

- Add branch prediction hints
- Add option `CQL_ENABLE_LOGGER_BY_DEFAULT` for testing
- Improve logging
	- Log sent and received message bytes for debug
	- Log thread id for every message
	- Colorize console logger
	- Improve log messages
- Disable enum to string conversion if EnumDescriptions specialization is not provided
- Use `seastar::data_sink` instead of `seastar::output_stream<char>` and reduce copy
- Enable keepalive for all connection by default and allow configure parameters

