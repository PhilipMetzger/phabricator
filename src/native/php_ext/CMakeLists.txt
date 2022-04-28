
# Abstract phpcpp away.
phab_lib(
    NAME php_ext
    SOURCES
    DEPS
    # Uses Exceptions, which are banned
    phpcpp::phpcpp
)
