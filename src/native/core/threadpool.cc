// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
#include "src/native/threadpool.h"

#include <string>
#include <thread>
#include <optional>
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

  // Is the worker detached and can it no longer be collected.
  bool detached() const { return detached_; }

  // What is our tid, used in `ThreadPool` for indexing.
  std::current_thread::id id() const;

  // Detaches the current Worker and runs it until `task_before_destruction`
  // returns true.
  void Detach(std::function<bool()> task_before_destruction);
 private:
  // Is the thread still running.
  bool is_joined_ = false;
  // Is the Worker running, as standalone. 
  bool detached_ = false;
  // Duration of the last sleep, used for finer grained scheduling.
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
  // TODO: Better API, for getting a Task.
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

void ThreadPool::WorkerThread::Detach(
        std::function<bool()> task_before_destruction) {
  CHECK(task_before_destruction) << "A callback must be passed to Detach";
  // Detach in `Main()`
  detached_ = true;
  // Move the task to tls.
  CHECK(!g_end_task);
  g_end_task = std::move(task_before_destruction);
  
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
  ~Watchdog() = default;


  // Collect idle and hanging threads. 
  // Returns the collected thread amount on success.
  // `std::nullopt` on failure.
  std::optional<int32_t> Collect();
  // Calculate the next Wakeup. 
  absl::Duration CalculateWakeUp() const;

  const ThreadPool* pool_; // Unowned, must outlive the Watchdog. 
  // The Time of the last wakeup. 
  std::optional<absl::Time> last_wakeup_;
};

void ThreadPool::Watchdog::Schedule() {
  // If this is the first schedule call from the ThreadPool, just sleep for 
  // `kBaseSchedule`.
  static bool first_run = last_wakeup_ == std::nullopt;  
  if (first_run) {
      ScheduleSleep();
      last_wakeup_ = absl::Now();
      absl::SleepFor(kBaseSchedule);
      return;
  }
  // TODO: heuristic scheduling for ThreadPool.threads_.size()
  auto base_duration = kBaseSchedule * pool_->threads_.size(); 
  auto additional_time = CalculateWakeUp();
  CHECK(base_duration + additional_time < absl::Minute(3));
}

void ThreadPool::Watchdog::Force() {

  // This can only run once. Before we shutdown the ThreadPool.
  CHECK(!g_force_called);
  g_force_called = true;
  // Collect the idle or hanging threads.
  auto num = Collect();
  CHECK(num.has_value());


}

ThreadPool ThreadPool(size_t concurrency) {
  for (size_t i = 0; i < concurrency; i++) {
    thread_.emplace(WorkerThread());
  }
  watcher_ = Watchdog::ForPool(this);
}

ThreadPool::~ThreadPool() = default;

void ThreadPool::PostJob(std::function<void()> job) {

  if (shutting_down()) {
      // If the ThreadPool is shutting down. Do `job` on a specific thread.
      ShutdownJob(std::move(job));
      return;
  }
  // TODO: locking
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

WorkerThread* ThreadPool::WorkerForId(std::thread::id id) {
  CHECK(!workers_.empty());

  return nullptr;

}
} // phab
