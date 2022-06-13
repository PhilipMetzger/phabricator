// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
#ifndef NATIVE_CORE_CONTEXT_H_
#define NATIVE_CORE_CONTEXT_H_

#include <cstdint>
#include <string>
#include <vector>


#include "absl/time/time.h"
#include "grpcpp/server.h"
#include "grpcpp/server_context.h"

#include "native/net/server.h"
#include "native/net/context.h"
#include "native/net/rpc_context.h"
#include "native/php_ext/context.h"

namespace phab::core {
// The `core::Context` is used to manage all registered services.
// It basically manages everything, from gRPC to HTTP. 
// The Design was taken from Go's `context.Context`.
//
// This class is threadsafe. 
class Context {
 public: 
  // Create a new Context. 
  static Context Create(bool debug, size_t global_concurrency);

  ~Context() = default;
  // Register a new grpc::Service. 
  void AddService(std::string_view name, const grpc::Server& server,
          const grpc::AsyncServerContext& ctx);
  
  // Set the Metric retention. 
  void SetMetricRetention(absl::Duration time);
  // Set the Crashhandler, which will be called on exit. The result of `crash`
  // will be written into the logs.
  void SetCrashHandler(std::function<void()> crash);
  // Return the global concurrency limit. 
  // It will be applied to each service.
  size_t max_global_concurreny() const { return max_global_concurrency_; }
  // Return the Deadline, before an answer fails.
  absl::Duration deadline() const { return deadline_; }
  // Return the current `Request`, it can be either RPC or HTTP.
  const net::Request& request() const { return network_context_.request(); }

  // Return the associated `stats_context`.
  const stats::Context& stats_context() const { return stats_context_; } 
  // Is this server running with io_uring as it's `EventLoop` base.
  // Returns true, if the server uses io_uring. 
  bool IoUring() const;
  // Is this Server running with epoll as it's `EventLoop` base.
  bool EPoll() const;
  // Return the associated `network_context`.
  const net::Context& network_context() const { return network_context_; }
  // Return the associated `grpc_context`.
  const rpc::Context& grpc_context() const { return grpc_context_; }
  // Return the associated `php_context`.
  const php_ext::Context& php_context() const { return php_context_; }

 private:
  // TODO: Move this to service_manager.{h,cc}
  // A managed gRPC Service, wich is named for debug purposes.
  class Service {
   public:
    Server(std::string_view name, const grpc::Service& service, 
            const grpc::ServerContext& ctx) :
        name_(name), service_(service), context_(ctx) {}
    ~Server() = default;

    std::string_view name() const { return name_; }
    bool health() const { return service_.healthcheck() != kUnhealthy; }

   private: 
    // The Service's name, e.g `Differential`
    std::string name_; 
    // The Service implementation. 
    grpc::Service service_;
    // The associated context.
    grpc::AsyncServerContext context_;

  };

  // The constructor.
  Context(const stats::Context& stat_context,
          const rpc::Cotnext& grpc_context,
          const net::Context& net_ctx, 
          const php_ext::Context& php_context,
          net::Server* net_server, 
          size_t max_concurrency);

  // Will be called before crashing. 
  std::function<void()> crash_handler_;
  // This holds all registered services. 
  std::vector<Service> managed_services_; 
  // All services are registered on this server.
  grpc::Server main_server_;
  // All ThreadPools which are used on this server.
  std::vector<ThreadPool*> thread_pools_;
  // The stats::Context is responsible for all stats.
  // It's used to calculate latency and throughput.
  stats::Context stats_context_;
  // The network context, is responsible for managing incoming request.
  net::Context network_context_;
  // The php context is responsible for managing the PHP ffi interface.
  // It knows how many PHP_LOG()'s are queued, or a a native call happens.
  php_ext::Context php_context_;
  // The associated server, which belongs to `network_context_`.
  // Not owning.
  net::Server* associated_server_;
  // The maximal amount of threads to create across all servers.
  size_t max_global_concurrency_;

};

}
