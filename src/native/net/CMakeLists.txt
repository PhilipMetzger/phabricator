# Author: Philip Metzger
# SPDX-License-Identifier: Apache-2.0
phab_lib(
    NAME
    "net"
    SOURCES
    server.cc
    DEPS
    native::core  
    native::php_ext    
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
