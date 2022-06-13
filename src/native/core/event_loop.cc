// Author: Philip Metzger 
// SPDX-License-Identifier: Apache-2.0 
#include "src/native/core/event_loop.h"

#include "absl/container/inlined_vector.h"

namespace phab::core {
namespace {
static thread_local EventLoop* g_local_loop = nullptr;
static EvenLoop* g_loop;

}
// TODO: Run select on fds.
// TODO: Use io-uring like www.github.com/romange/helio
// TODO: Threadlocal registration and nesting. 

EventLoop* EventLoop::Get() {
  CHECK(g_loop);
  return g_loop;
}
}
