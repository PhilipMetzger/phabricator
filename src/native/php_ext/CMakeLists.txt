# Author: Philip Metzger
# SPDX-License-Identifier: Apache-2.0

# Abstract phpcpp away.
phab_lib(
    NAME 
    php_ext
    SOURCES
    context.cc
    value.cc
    class.cc
    DEPS
    # Uses Exceptions, which are banned
    phpcpp::phpcpp
)

phab_lib(
    NAME
    php_ext_test_support
    SOURCES
    class_mock.cc
    value_testutil.cc
    context_test_util.cc
    DEPS
    php_ext
)

phab_test(
    NAME
    php_ext_test
    DEPS
    phpcpp::phpcpp
)
