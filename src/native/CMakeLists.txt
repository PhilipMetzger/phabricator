# Author: Philip Metzger
# SPDX-License-Identifier: Apache-2.0
# PhabNative
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

# Common compiler flags, adjust via CMakePresets.json for your own.
add_compile_options(-fno-exceptions)
add_compile_options(-fno-rtti)
add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Wformat=2)

# PHP-CPP Abstraction Layer
# As this third_party project requires exceptions.
add_subdirectory(php_ext)
if (BUILD_TESTS)
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
#          [Private]
#           -> differential_grpc -> php_ext
#           -> harbormaster_grpc -> php_ext
#           -> diffusion_grpc -> php_ext
phab_bin(
    NAME native 
    SOURCES main.cc 
    LIBS server
)


