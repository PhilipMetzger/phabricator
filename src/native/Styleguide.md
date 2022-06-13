# C++ Styleguide

As mentioned in the [Readme](Readme.md), we follow the Google C++ Styleguide.

## Context

This project implements a Go like `context.Context` API, which are passed in to
API's. 

On Server startup the `core::Context` is created, which owns all subcontext, 
these are module specific, so `net::Context` contains the last requests 
deadline and other settings. 

Notable Contexts: 
 - `core::Context`, The PhabNative Global State.
 - `net::Context`, the networking context, used for the http server.
 - `rpc::Context`, common configuration for the gRPC services.
 - `php_ext::Context`, the PHP ffi Context, responsible for `PHP_LOG(type)`.
 
## Async Code and Threading

You can usually find a `phab::ThreadPool` to post tasks to. The ThreadPool has 
a Chromium inspired API, which allows posting tasks sequentially and unordered.

This is the full API:
```c++
namespace phab {
// Snippet of the relevant API's 
class ThreadPool {
 public:
  // Post a Task to this ThreadPool with no ordering guarantees.
  void PostTask(std::function<void()> job);
  // Same as above, but it must be done within the deadline.
  void PostTaskWithDeadline(std::function<void()> job, absl::Duration deadline);

  // Post a Task to this ThreadPool with a ordering guarantee. 
  // If Task A arrives before Task B, A will be executed before B.
  void PostSequencedTask(std::function<void()> job);

  // Post a Task, which will be executed on multiple Threads simultanously, no
  // ordering guarantees.
  void PostParallelTask(std::function<void()> job);

};
//...
}
```

Always use `absl::bind_front()` to create a task. 

## gRPC

We always generate services with the new Callback based API, which are based on
the Google internal API.

It's forbidden to block in RPC Code.

