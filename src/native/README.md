# Phabricator Native Libraries

This directory contains all native implementations for an improved Phabricator.

Notably this contains the new gRPC based HarborMaster and PhabServer.


## Architecture
Each Phabricator Application which has a native rewrite, gains a native and api 
subdirectory, which contain the corresponding .proto and .cc/.h files.

The native Implementations will be exported to php via ffi, so the usual 
controllers can call them. 

The Frontend will be untouched. 

## Important Subdirectories

- cmake, CMake support.
- php_ext, PHP Extensions support.
- server, Server implementation.
- test, test support, e.g `testing::Request`. 
- third_party, Third Party Libraries, which are vendored.

## Adding new Code

We follow the Google Styleguide for C++ Code. Additionally we never interface 
directly with the database, because the PHP Side implements strict checking via
Policy.

Every class must have a unittest in the matching `_test.cc` file.

### Security Guidelines

Follow the [rule of two](https://chromium.googlesource.com/chromium/src/+/master/docs/security/rule-of-2.md).

And sanitize inputs.

## Additional Info

This work is inspired by [Gerrit](www.gerritcodereview.com) and [LUCI](www.github.com/luci/luci-go).
Which are both used for [Chromium](www.chromium.org) Infrastructure.
