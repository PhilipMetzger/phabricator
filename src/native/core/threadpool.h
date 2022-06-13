// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
#ifndef SRC_NATIVE_THREADPOOL_H_
#define SRC_NATIVE_THREADPOOL_H_

#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

namespace phab {

// A classic Threadpool, which may be backed by a smarter Executor.
// Post work via `PostJob`.
class ThreadPool {
 public: 
  // Create a ThreadPool with `concurrency` threads.
  explicit ThreadPool(size_t concurrency);
  ~ThreadPool();

  // Post a Job, which will be run in a background thread. 
  // A `job` may be reordered in the job queue.
  void PostJob(std::function<void()> job);

  // Post a Job which will be sequentally executed, this will be done with
  // in the `deadline`. 
  void PostSequencedJob(std::function<void()> job, absl::Duration deadline);

  // Shutdown the ThreadPool. The Threadpool is ready to destruct, after 
  // either `condition` is met or `deadline` is exceeded.
  void Shutdown(absl::Condition condition, absl::Duration deadline);

  // Does this ThreadPool have any pending or queued tasks.
  // This is threadsafe.
  bool HasTasks() const; 

  // TODO: Parallel API, for `net::Server::Shutdown()`
 private:
  // `ThreadPool::State` encapsulates all shared state. 
  // It must be loaded atomically. 
  struct State {
    // All queued jobs, gets filled by the public API.
    std::vector<Job> queued_jobs;
    // The current sequence number.
    std::atomic<int32_t> sequence;
    // The per thread queue, allows work-stealing.
    thread_local std::vector<Job> thread_queue;

  };
  // The Watchdog periodically awakes to restart hanging or blocked threads.
  class Watchdog; 
  // A simple Workerthread.
  class WorkerThread;
  // A parallel Worker.
  class ParallelWorker;

  // Post a Job which is run shortly before shutting down.
  void PostShutdownJob(std::function<void()> job);
  
  // Notify the Workers about a new task. 
  void NotifyWorkers(); 

  bool shutting_down() const { return jobs_.is_empty() && threads_.length() < 2; }

  // Return the Worker for a thread id. 
  // Used in `PostShutDownJob` to keep a Worker alive.
  WorkerThread* WorkerForId(std::thread::id id);

  // Name of the Threadpool.
  std::string name_;
  
  // The Watchdog is a separate thread, controlling the congestion of the
  // ThreadPool, as in joining idle threads or marking them for reuse.
  // It may be awakened during a `PostJob*` API call or from it's own
  // scheduling.
  Watchdog* watcher_;
  // TODO: Protect these with a absl::Mutex, or create a Mutex container.
  // All Workers in the Pool.
  std::vector<WorkerThread*> threads_;
  std::vector<WorkerThread*> idle_threads_;
  std::vector<ParallelWorker*> parallel_workers_;
  // Posted Jobs in FIFO order. Tasks must be tracked externally.
  std::vector<std::function<void()>> jobs_;

};

} // phab
#endif // SRC_NATIVE_THREADPOOL_H
