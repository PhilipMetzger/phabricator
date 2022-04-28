# Author: Philip Metzger
# SPDX-License-Identifier: Apache-2.0
phab_proto(
    NAME 
    "differential_proto"
    SOURCES
    v1/revision.proto
)

phab_grpc(
    NAME
    "differential_service"
    SOURCES
    v1/differential.proto
    PROTO_DEPS
    differetial_proto
    GENERATE_MOCK
)
