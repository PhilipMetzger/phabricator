// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
#include "src/native/net/server.h"

#include "src/native/metric.h"

#include "absl/time/time.h"
namespace phab::net {
namespace {

EventLoop* g_loop = nullptr;
constexpr absl::Duration kPoolShutdown = absl::Minute(1);
}
// static 
Server Server::Create(size_t max_concurrency) {
  auto pool = Threadpool(max_concurrency);
  CHECK(!g_loop);
  auto loop = EventLoop::Create();
  g_loop = &loop;
  auto socket = Socket::Open(80);
  return Server(std::move(socket),std::move(loop),std::move(pool));
}

void Server::Run() {
  loop_.Exec();
  // This is run before `EventLoop::Quit()`, so you are able to post Jobs until
  // `Shutdown()` is run.
  if (loop_.QuitNotified()) {
    RunRemainingTasks();
    // EventLoop is shutdown.
    Shutdown();
  }
}

void Server::RunRemainingTasks() {
  while(pool_.HasTasks()) {
    // Force parallel running of Tasks.
    // This may lead to thread starvation.
    pool_.SplitTasks();
  }
  // Wait until no jobs are running or a minute has passed.
  pool_.Shutdown(+[this]() { return !pool_.HasTasks(); },kPoolShutdown);
}

void Server::Shutdown() {
  // The Eventloop must have gotten the quit signal.
  CHECK(loop_.has_quit());
  for (const auto& [peer_name,socket] : peers_) {
    LOG(INFO) << "Shutting down, socket for " << peer_name;
    // Disconnect from the peer.
    socket.Close();
    
  }
  
}

}
