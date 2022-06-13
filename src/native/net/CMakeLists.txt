# Author: Philip Metzger
# SPDX-License-Identifier: Apache-2.0

# This is the public API of native/net
phab_lib(
    NAME
    "net"
    SOURCES
    context.cc
    server.cc
    DEPS
    native::core  
    native::php_ext    
)

# For internal use.
phab_lib(
    NAME
    "net_internal"
    SOURCES
    socket.cc
    DEPS
    absl::status
    absl::string
    native::core
)

phab_test(
    NAME
    "net_test"
    SOURCES
    server_test.cc
    DEPS
    net
    testing::server
    testing::request
)

phab_test(
    NAME
    "net_internal_test"
    SOURCES
    socket_test.cc
    iobuffer_test.cc
    DEPS
    net_internal
)
