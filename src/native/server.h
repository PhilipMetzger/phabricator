// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
#ifndef SRC_NATIVE_SERVER_H_
#define SRC_NATIVE_SERVER_H_

#include <cstdint>
#include <functional>
#include <string>
#include <string_view>

#include "src/native/core/context.h"
#include "src/native/net/response.h"
#include "src/native/net/server.h"
#include "src/applications/differential/native/server.h"
#include "src/applications/harbormaster/native/server.h"

#include "absl/containers/flat_hash_map.h"

namespace phab {

class Server {
 public:
  // Reexport core::Context as phab::Server::Context.
  using Context = core::Context;

  // This allows user to pass stateful callbacks. 
  // The response is written on a background thread.
  using ResponseCallback = std::function<void(Context&,http::Response*)>;

  struct Options {
    // Allow debug endpoints under "host/debug/".
    // This is insecure and may leak private data.
    bool debug;
    // The domain to host on, iff empty localhost:80 or ::1.
    std::string domain;
    // A Callback to invoke on server shutdown. 
    // Must be threadsafe, because it may be called on any thread.
    std::function<void(const Context&)> on_shutdown;
    // The maximal amount of threads to use. 
    size_t max_concurrency;
  }; 

  // Create the Phabricator Server.
  static Server Create(Options& opts);

  ~Server() = default;

  // Registers all known gRPC Services.
  // Can only be called once.
  void RegisterServices();
  // Registers a toplevel http handler, `endpoint` must outlive the server.
  void Register(std::string_view endpoint, ResponseHandler callback);
  // Symbolizes the current stack and 
  void Crash() const;
  // Run the server. 
  void Run();

  Context& context() { return context_; } 
  const Context& context() const { return context_; }
  
 private:

  Server(const core::Context& ctx,
         const absl::flat_hash_map<std::string_view,ResponseHandler> defaults,
         const net::Server& server) 
      : context_(ctx),
        handlers_(std::move(defaults)),
        server_(std::move(server)) {}

  Server(const core::Context& ctx,const net::Server& server) : 
      context_(ctx),
      server_(server) {}
  // The Context, for all things which the server is concerned about.
  core::Context context_;
  // All toplevel response handlers, they must be registered on startup.
  absl::flat_hash_map<std::string_view,ResponseCallback> handlers_;
  // The actual http server, which contains a Eventloop. 
  net::Server server_;
};



}


#endif
