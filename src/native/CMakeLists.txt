# Author: Philip Metzger
# SPDX-License-Identifier: Apache-2.0
#
# PhabNative is the gRPC based implementation.
cmake_minimum_required(VERSION 3.18...3.22)
project("PhabNative" LANGUAGES CXX)


# Use C++17
set(CMAKE_CXX_STANDARD 17)
# Disallow extensions
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_BIN_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "In source builds are not allowed, delete the generated CMakeFiles and use a build dir")
endif()

# Inform CMake about our modules.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Use the Adresssanitizer
option(USE_ASAN OFF)
# Use TSAN, the Threadsanitizer
option(USE_TSAN OFF)
# Use MSAN, the Memorysanitizer
option(USE_MSAN OFF)
# Build Tests, off for Release
option(BUILD_TESTS OFF)
# Use vendored sources in /third_party/ instead of system-installed.
# Some paths will change from "absl/" to "third_party/absl"
option(USE_VENDORED OFF)

# Common compiler flags, adjust via CMakePresets.json for your own.
add_compile_options(-fno-exceptions)
add_compile_options(-fno-rtti)
add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Wformat=2)
if(USE_ASAN)
    add_compile_options(-fsanitize=address)
    add_definitions(USE_ASAN)
elseif(USE_TSAN)
    add_compile_options(-fsanitize=thread)
    add_definitions(USE_TSAN)
elseif(USE_MSAN)
    add_compile_options(-fsanitize=memory)
    add_definitions(USE_MSAN)
endif()
# Hammer is to big atm, split into vendor_project.
# Vendored deps:
# Absl
# gRPC
# Googletest
# protobuf
# php-cpp
if(USE_VENDORED_SOURCES)
    add_defines(USE_VENDORED)
else()
    find_package(Protobuf CONFIG REQUIRED)
    find_package(gRPC CONFIG REQUIRED)
    find_package(PHPCPP CONFIG REQUIRED)
    find_package(GoogleTest CONFIG REQUIRED)
endif()
# PHP-CPP Abstraction Layer
# As this third_party project requires exceptions.
add_subdirectory(php_ext)
if(BUILD_TESTS)
    # gRPC and server testing tools
    add_subdirectory(test)
endif()
# third_party projects
add_subdirectory(third_party)
# server entry point
add_subdirectory(server)

# Dependency Layering 
# native -> server [Private] -> net 
#                            -> absl
#                  [Private]
#                            -> differential_grpc -> php_ext
#                            -> harbormaster_grpc -> php_ext
#                            -> diffusion_grpc -> php_ext
phab_bin(
    NAME 
    "phab-native"
    SOURCES     
    main.cc 
    server.cc
    PHAB_DEPS 
    server
)
include(GNUInstallDirs)
# The only exported target.
install(COMPONENT phab-native DESTINATION ${CMAKE_INSTALL_BINDIR}
                              RUNTIME DESTINATION bin)
