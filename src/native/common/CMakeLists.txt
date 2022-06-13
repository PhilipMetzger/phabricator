# Author: Philip Metzger
# SPDX-License-Identifier: Apache-2.0
phab_lib(
    NAME
    "common"
    SOURCES
    DEPS
    native::core
    absl::strings
    absl::status
    absl::containers
)

phab_test(
    NAME
    "common_test"
    SOURCES
    DEPS
    common
)
