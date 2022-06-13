// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
#include "src/native/net/context.h"

#include <memory>

#include "src/native/net/server.h"
#include "src/native/net/response.h"

namespace phab::net {

Context Context::Create(net::Server* server) :
    server_(server)
    {}

~Context::Context() = default;


std::unique_ptr<http::Request> Context::CreateResponse() { 
  // Create a new Response. 
  auto new_response = std::make_unique<http::Response>();
  // Add it to all responses.
  requests_.emplace(&new_response);
  return new_response;
}
}
