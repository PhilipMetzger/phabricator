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
    cmake_parse_arguments(bin_args "" "NAME" "SOURCES;PHAB_DEPS;DEPS" ${ARGN})
    if(NOT bin_args_NAME)
        message(FATAL_ERROR "A Name must be set for this binary.")
    endif()
    add_executable(${bin_args_NAME} ${bin_args_SOURCES})
    if(bin_args_PHAB_DEPS)
        # Phabricator are all internal, so no CMake namespacing required.
    endif()
    if(bin_args_DEPS)
        foreach(ext_dep IN LIST bin_args_DEPS)
            target_link_libraries(${bin_args_NAME} PRIVATE ${ext_dep})
        endforeach()
    endif()
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
    cmake_parse_arguments(lib_args "" "NAME" "DEFINES;DEPS;PUBLIC_DEPS;PUBLIC_DEFINES" ${ARGN})
    # Emulate Chromiums component.
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_library(${lib_args_NAME} STATIC)
    else()
        add_library(${lib_args_NAME} SHARED)
    endif()
    # Alias native/differential => native::differential
    add_library("${NS_DIRECTORY}::${lib_args_NAME}" ALIAS ${lib_args_NAME})
endfunction()

# Create a test
# 
# Arguments:
#   name: name of the test.
#   test_libs: additional test libraries to link.
#   custom_main: (optional) main to use instead of GTest::Main.
#   deps: libraries to link
function(phab_test)
    cmake_parse_arguments(test_args "" "NAME;CUSTOM_MAIN" "TEST_LIBS;DEPS" ${ARGN})
    # Shared libs for faster relinking.
    add_library(test_args_name SHARED)
    target_link_library(${test_args_NAME} PRIVATE GTest::gtest GMock::gmock)
    if(test_args_TEST_LIBS)
        foreach(test_lib in LIST test_args_TEST_LIBS)
            target_link_library(${test_args_NAME} PRIVATE ${test_lib})
        endforeach()
    endif()
    target_linK_library(${test_args_NAME} PRIVATE $<${test_args_CUSTOM_MAIN}:${test_args_CUSTOM_MAIN},GTest::gtest_main>)
    gtest_discover_tests(${test_args_NAME})
endfunction()

# Create a Protobuf library, the generated protos will be linked 
# as object library, to reduce compile time.
# 
# Arguments:
#   name: name of the library
#   sources: proto sources, required
#   deps: other imported protos
function(phab_proto)
    cmake_parse_arguments(proto "" "NAME" "SOURCES;DEPS" ${ARGN})
    # TODO: Ellide api::v1::proto => api::proto 
    set(outputs "")
    if (NOT ${proto_NAME} OR NOT ${proto_SOURCES})
        message(FATAL_ERROR "Name and Sources must be set for phab_proto")
    endif()
    add_library(${proto_NAME} OBJECT "${outputs}")    
    # Note: Protos are always passed as relative source. 
    # This means they always get generated in build/service/api/v1/name.pb.{h,cc}
    target_include_directories(${proto_NAME} INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/${VERION_DIR}/${outputs}")
    add_library("${NS_DIRECTORY}::${proto_NAME}" ALIAS ${proto_NAME})
endfunction()

# Create a grpc service. 
# 
# Arguments:
#   name: name of the service
#   source: a single protobuf.
#   proto_deps: (optional), a list of protobuf dependencies.
#   generate_mock: (optional), generate a service mock.
function(phab_grpc)
    cmake_parse_arguments(grpc "GENERATE_MOCK" "NAME;SOURCE" "PROTO_DEPS" ${ARGN})
    # Same rules as above.
    if(grpc_GENERATE_MOCK)
        list(APPEND ${OUTPUT} "${BASENAME}.mock.grpc.cc")
    endif()
    add_library(${grpc_NAME} OBJECT ${OUTPUT})
    target_include_directories(${grpc_NAME} INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/${VERSION_DIR}/${OUTPUT}")
    if(EXISTS grpc_PROTO_DEPS)
        foreach(protodep IN LIST grpc_PROTO_DEPS)
            target_set_property(${grpc_NAME} DEPENDS ${protodep})
        endforeach()
    endif() 
    add_library("${NS_DIRECTORY}::${grpc_NAME}" ALIAS ${grpc_NAME}) 
endfunction()

# Create a gRPC library. gRPC is automatically added.
# 
# Arguments:
#   name: name of the library
#   service: required, the generated grpc service.
#   sources: the sources, required
#   deps: linked libraries
function(phab_grpc_lib)
    cmake_parse_arguments(grpc_lib "" "NAME;SERVICE" "SOURCES;DEPS" ${ARGN})
    target_link_library(grpc_lib_NAME PRIVATE Grpc::Grpccpp Protobuf::LibProtobuf)
    if(grpc_lib_DEPS)
        foreach(dep IN LIST grpc_lib_DEPS)
            target_link_library(${grpc_lib_NAME} PRIVATE ${dep})
        endforeach()
    endif()
endfunction()

