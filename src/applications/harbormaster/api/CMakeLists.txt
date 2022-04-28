# Author: Philip Metzger
# SPDX-License-Identifier: Apache-2.0
phab_proto(
    NAME
    "harbormaster_proto"
    SOURCES
    v1/build.proto
    v1/backend.proto
)

phab_grpc(
    NAME
    "harbormaster_service"
    SOURCES
    v1/harbormaster.proto
    PROTO_DEPS
    api::harbormaster_proto
    CREATE_MOCK 
)
