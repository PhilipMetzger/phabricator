# Author: Philip Metzger
# SPDX-License-Identifier: Apache-2.0

# Phabricator native utilities
# These CMake rules are very bazel-like.

# Create a binary
# 
# Arguments:
#   name: the name of the target
#   sources: the sources of the binary
#   phab_deps: phabricator libraries
#   deps: third_party or system libraries
function(phab_bin)
    cmake_parse_arguments(bin_args ${ARGN})
endfunction()

# Create a library
# 
# Arguments: 
#   name: name of the target
#   sources: sources of the library
#   defines: local definitions
#   deps: non propagated libraries
#   public_deps: propagated libraries
#   public_defines: propagated defines 
function(phab_lib)
    cmake_parse_arguments(lib_args ${ARGN})
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_library($lib_args_name STATIC)
    else()
        add_library($lib_args_name SHARED)
    endif()
endfunction()

# Create a test
# 
# Arguments:
#   name: name of the test.
#   test_libs: additional test libraries to link.
#   custom_main: (optional) main to use instead of GTest::Main.
#   deps: libraries to link
function(phab_test)
    cmake_parse_arguments(test_args ${ARGN})
    # Shared libs for faster relinking.
    add_library(test_args_name SHARED)
    target_link_library(test_args_name PRIVATE GTest::GTest GMock::GMock GTest::Main)
endfunction()

# Create a Protobuf library, the generated protos will be linked 
# as object library, to reduce compile time.
# 
# Arguments:
#   name: name of the library
#   sources: proto sources, required
#   deps: other imported protos
function(phab_proto)
    cmake_parse_arguments(proto ${ARGN})
    # TODO: Ellide api::v1::proto => api::proto
    add_library(proto_name OBJECT)
endfunction()

# Create a grpc service. 
# 
# Arguments:
#   name: name of the service
#   source: a single protobuf.
#   proto_deps: (optional), a list of protobuf dependencies.
#   generate_mock: (optional), generate a service mock.
function(phab_grpc)
    cmake_parse_arguments(grpc ${ARGN})
endfunction()

# Create a gRPC library. Grpccpp is automatically added.
# 
# Arguments:
#   name: name of the library
#   service: required, the generated grpc service.
#   sources: the sources, required
#   deps: linked libraries
function(phab_grpc_lib)
    cmake_parse_arguments(grpc_lib ${ARGN})
    target_link_library(grpc_lib_NAME PRIVATE GRPC::GRPCCPP Protobuf::LibProtobuf)
endfunction()

