// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
#include "src/native/threadpool.h"

#include <string>
#include <thread>
#include <vector>

namespace phab {

class ThreadPool::WorkerThread {
 public: 
   WorkerThread() : thread_base([this]() { this->Main(); }) {} 
  ~WorkerThread() = default;
  // `Main` is run in the underlying std::thread.
  // This does actual work.
  void Main();

  void set_joined(bool yes) { is_joined = yes; }

  // Is this worker thread joined, or can we re-use it.
  bool joined() const { return is_joined_; }

  // What is our tid, used in `ThreadPool` for indexing.
  std::current_thread::id id() const;
 private:
  // Is the thread still running.
  bool is_joined_ = false;
  // Duration of the last sleep, used for finer schedules.
  absl::Duration sleep_;
  // The underlying thread.
  std::thread thread_base_;

};

void ThreadPool::WorkerThread::Main() {

  if (awakened()) {
    sleep_ = last_sleep - absl::Now();
  }
  if (joined()) {
      // Nothing to do.
      return;
  }
  auto& task = Context::Get().Executor().next_task();
  if (task.empty()) {
      auto metric = common::Metric("phab.core.ThreadPool.Worker.sleep");
      auto recommended_sleep = Context::Get().sleep_duration();
      thread_base_.sleep(recommended_sleep);
    
  }
}

std::current_thread::id ThreadPool::WorkerThread::id() const {
  return thread_base_.id();
}

class ThreadPool::Watchdog {
 public:
  // Create a Watchdog for `pool`.
  static Watchdog ForPool(ThreadPool* pool); 
  // Schedule a collection.
  void Schedule();

  // Force a run. This API should only be called once.
  void Force();

 private:  
  Watchdog(ThreadPool* pool) : pool_(pool) {}

  const ThreadPool* pool_; // Unowned, must outlive the Watchdog. 
  // The Time of the last wakeup. 
  std::optional<absl::Time> last_wakeup_;
};

ThreadPool ThreadPool(size_t concurrency) {
  for (size_t i = 0; i < concurrency; i++) {
    thread_.emplace(WorkerThread());
  }
  watcher_ = Watchdog::ForPool(this);
}

ThreadPool::~ThreadPool() = default;

void ThreadPool::PostJob(std::function<void()> job) {
  // TODO: locking
  if (is_shutting_down()) {
      // If the ThreadPool is shutting down.
      ShutdownJob(std::move(job));
  }
  jobs_.emplace(std::move(job));
  NotifyWorkers();
  watcher_.Schedule();

}

void ThreadPool::PostSequencedJob(std::function<void()> job, 
        absl::Duration deadline) {

}

void ThreadPool::PostShutdownJob(std::function<void()> job) {
   // TODO: Determine the last job.  
}
}

