// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
#include "src/native/server.h"

#include "src/core/context.h"
#include "src/net/server.h"

#include "absl/debug/symbolize.h"

namespace phab {
namespace {
// Are all rpc services registered.
bool g_initialized_services = false; 
constexpr int kInsecure = 80;
} // namespace
Server Server::Create(Options& opts) {
  core::Context ctx = core::Context(opts.debug,opts.domain,
          opts.max_concurrency);

  auto server = net::Server::Create(ctx.host(),kInsecure);

  return Server(std::move(ctx),std::move(server));
}

void Server::RegisterRoot(ResponseHandler handler) {
  CHECK_EQ(handlers_.find("/"),handlers_.end()) << "Overwriting "/" 
      "is not supported";
  handler_.emplace("/",std::move(handler));

}

void Server::Crash() const {
  void* pc = nullptr;
  std::string out;
  if (!absl::Symbolize(&pc,&out))
      return;
  LOG(FATAL) << out;
  IMMEDIATE_CRASH(); 

}

void Server::RegisterServices() {
  CHECK(!g_initialized_services);
  for (const auto& service : known_services) {
    context_.RegisterService(service.name,service.server,service.context);
  }
  g_initialized_services = true;
}

} // namespace phab
