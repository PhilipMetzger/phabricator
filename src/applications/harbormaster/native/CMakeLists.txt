# Author: Philip Metzger
# SPDX-License-Identifier: Apache-2.0
phab_grpc_lib(
    NAME
    "harbormaster"
    SERVICE
    api::harbormaster_service
    PROTO_DEPS
    api::harbormaster_proto
    SOURCES
    backend_support.cc
    harbormaster_impl.cc
    harbormaster_support.cc
    DEPS 
    native::php_ext
    absl::containers # flat_hash_{set,map}
    absl::functional # function_ref, bind_front
    absl::strings 
)

phab_test(
    NAME
    "harbormaster_test"
    SOURCES
    harbormaster_impl_test.cc
    harbormaster_support_test.cc
    harbormaster_mock.cc
    DEPS
    harbormaster
    native::php_ext
)
