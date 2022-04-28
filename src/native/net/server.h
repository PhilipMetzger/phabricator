// Author: Philip Metzger
// SPDX-License-Identifer: Apache-2.0
#ifndef SRC_NATIVE_NET_SERVER_H_
#define SRC_NATIVE_NET_SERVER_H_

#include <cstdint>
#include <functional>

#include "src/native/core/event_loop.h"
#include "src/native/core/threadpool.h"
#include "src/native/http/socket.h"

#include "absl/functional/bind_front.h"

namespace phab::http {

// http::Server abstracts a simple http server.
// It's responsible for its own Threadpool.
// Each response is separately allocated, by the http::Context.
// The ordering of a Request-Response pair is guaranteed.
class Server {
 public:  
  // Create a new Server with `max_concurrency` threads.
  static Server Create(size_t max_concurrency);
  ~Server();

  // Enable or disable the Nagle algorithm. 
  // This is responsible for 40ms buffering in requests.
  void set_nagle(bool on) { socket_.set_nagle() = on; }
  bool nagle() const { return socket_.nagle(); }
  // Run until the owned EventLoop received a Quit signal.
  void Run();
  // Connect to a peer, which is an valid IPAddress.
  // It tries this up `max_retries` times.
  bool Connect(std::string_view peer, size_t max_retries);
 private:
  // Models a Http Connection state machine, for more info see `DoConnectLoop()`.
  enum class Connection : uint8_t {
      kStarted, 
      kConnected,

  };
  // Models a state machine for reading requests. See `DoReadLoop()` for 
  // further information.
  enum class Response: uint8_t {};

  Server(Socket socket,EventLoop& loop,ThreadPool& pool);

  Server(Server&) = delete;
  Server operator=(Server&) = delete;
  Server(Server&&) = delete;
  Server operator=(Server&&) = delete;
 
  void Respond(core::Context& context, 
          absl::FunctionRef<void(core::Context&, http::Response*)> func) {
    http::Response response;
    func(context,&response);
    absl::Duration deadline = context.deadline();
    // This is safe as it only is called when the server is alive.
    pool_.PostJob(absl::bind_front(Server::Write,this,
                std::move(response),deadline));

  }
  // Read the current incoming request into `response`.
  void Read(const core::Context& context, http::Response& response);
  // Write the `response` within `deadline`.
  void Write(const http::Response& response, absl::Duration deadline);
  // Shutdown the server. Must run on the main thread.
  void Shutdown(); 
  // Run all remaining tasks in the `pool_`.
  void RunRemainingTasks();

  // All connected clients.
  absl::flat_hash_map<std::string_view,Socket> peers_;
  // Threadpool of background threads.
  Threadpool pool_;
  // The servers socket.
  Socket socket_;
  // Eventloop, lives as long as the server.
  EventLoop loop_;
  
};


}

#endif
