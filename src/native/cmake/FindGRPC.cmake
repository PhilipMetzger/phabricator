# Three Step Find:
# pkg-config
# find_library
# Error, use vendored library in /third_party/grpc
#
# You can opt-in to use the vendored library with -DUSE_GRPC_VENDOR
option(USE_GRPC_VENDOR true) # TODO: remove for other users

# Step: 1, pkg-config
# Step: 2, search local libs and includes
set(GRPC_FOUND false)
find_library(GRPC OUTPUT_VARIABLE ${GRPC_FOUND})

if (USE_GRPC_VENDOR)
    set(GRPC_FOUND true)
    set(GRPC_INCLUDE_DIR "${CMAKE_PROJECT_SOURCE_DIR}/third_party/grpc/include")
endif()
