// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
#include "src/native/net/context.h"

#include <memory>

#include "src/native/net/server.h"
#include "src/native/net/request.h"

namespace phab::net {

Context Context::Create(net::Server* server) :
    server_(server)
    {}

~Context::Context() = default;


}
