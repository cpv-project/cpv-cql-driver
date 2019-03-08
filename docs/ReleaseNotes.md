# Release Notes

## 0.5

Changes:

- Update build script and cmake files, build as shared library
- Reference seastar from pkg-config
- Provide ubuntu package

## 0.4

Changes:

- Add metrics support

## 0.3

Changes:

- Improve concurrent performance
- Fix task race in connection pool
- Add concurrent benchmarks

## 0.2

New features:

- Support prepared query, see [Prepare](https://github.com/cpv-project/cpv-cql-driver/blob/master/docs/Prepare.md)
- Add [SessionConfiguration::setDefaultConsistency](https://github.com/cpv-project/cpv-cql-driver/blob/master/docs/Configuration.md#setdefaultconsistencyconsistencylevel)
- Add [SessionConfiguration::setPrepareAllQueries](https://github.com/cpv-project/cpv-cql-driver/blob/master/docs/Configuration.md#setprepareallqueriesbool)
- Support frame compression, see [NodeConfiguration::setUseCompression](https://github.com/cpv-project/cpv-cql-driver/blob/master/docs/Configuration.md#setusecompressionbool)
- Require atleast c++17 by default

Api Changes:
See #15

## 0.1

The first release.
Features:

- Futures based API
- Connection pooling
- Connection multiplexing
- Automatic retry
- Load balancing
- Authentication
- SSL
- Batch Operation

