// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
#ifndef SRC_NATIVE_THREADPOOL_H_
#define SRC_NATIVE_THREADPOOL_H_

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
 private:
  // The Watchdog periodically awakes to restart hanging or blocked threads.
  class Watchdog; 
  // A simple Workerthread.
  class WorkerThread;

  // Post a Job which is run shortly before shutting down.
  void PostShutdownJob(std::function<void()> job);

  bool is_shutting_down() const { return jobs_.is_empty() && threads_.length() < 2; }

  // Return the Worker for a thread id. 
  WorkerThread* WorkerForId(std::thread::id id);

  // Name of the Threadpool.
  std::string name_;
  
  Watchdog watcher_;
  // TODO: Protect these with a absl::Mutex, or create a Mutex container.
  std::vector<WorkerThread> threads_;
  // Posted Jobs in FIFO order. Tasks must be tracked externally.
  std::vector<std::function<void()>> jobs_;

};

} // phab
#endif // SRC_NATIVE_THREADPOOL_H
