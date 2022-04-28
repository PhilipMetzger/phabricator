# What is Third-Party
This directory contains all third-party code which is used for Phabricator.
Each `third_party` Project must contain a LICENSE and a CMakeLists.
Most of these are vendored for an easier Project Setup.

## One-Version
Each dependency must be only added once.
Multiple Versions are not allowed.

For each dependency, a Readme.phab is required.
How it looks 
```text
Abseil, Google commons library
Version: 2021-12-24 LTS
Security: Critical
Last-Update-Date: Upgraded on 2022-04-13
```

## Approved Dependencies
- Abseil, see `absl`
- PHP-CPP, see `php-cpp`
- GRPC, see `grpc`
- Protoc, used by grpc, see `protobuf`
- Googletest, GMock + GTest used for native tests, see `googletest`

## Using System-Libraries
Phabricator Native uses them by default, you can opt out by passing
```cmake 
cmake -S . -B build -DUSE_VENDOR
```
except Abseil, which must be vendored.
