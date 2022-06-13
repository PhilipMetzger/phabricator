// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
#ifndef SRC_NATIVE_NET_CONTEXT_H_
#define SRC_NATIVE_NET_CONTEXT_H_

#include <cstdint>
#include <memory>
#include <vector>

namespace phab::http {

// A Context for managing mostly `http::Request` and `http::Response`.
// Should be used for passing Requests to the Server.
class Context {
 public: 
  // Create a new Context
  explicit Context(http::Server* server);
  ~Context();
  // Create a new `http::Response`, the caller is responsible for passing it to
  // the server.
  std::unique_ptr<http::Response> CreateResponse(); 
  // Return all open `requests`
  const std::vector<http::Request>& requests() const { return requests_; }

  size_t num_connections() const { return server_->OpenConnections(); }
  const http::Server* server() const { return server_; }
 private:
  std::vector<http::Response*> inflight_responses_;
  std::vector<http::Request> requests_;
  http::Server* server_;

};
} // phab::http
#endif // SRC_NATIVE_NET_CONTEXT_H_
