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

- CMake, CMake support.
- php_ext, PHP Extensions support.
- server, Server implementation.
- test, test support, e.g `testing::Request`. 
- third_party, Third Party Libraries, which are vendored.

## Additional Info

This work is inspired by [Gerrit](www.gerritcodereview.com) and [LUCI](www.github.com/luci/luci-go).
Which are both used for [Chromium](www.chromium.org) Infrastructure.
