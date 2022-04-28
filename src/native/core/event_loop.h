// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
#ifndef SRC_NATIVE_CORE_EVENT_LOOP_H_
#define SRC_NATIVE_CORE_EVENT_LOOP_H_

#include "absl/container/inlined_vector.h"

namespace phab::core {

// The `EventLoop` is responsible for keeping the `phab::Server` responsive.
// Usually just one exists.
// Eventloops can be nested, to support batching workflows.
class EventLoop {
 public:
  // `WorkHandle` represents any type of Work in the EventLoop.
  class WorkHandle {
   public:
     
     virtual ~WorkHandle();

     // Do the actual Work, this may be writing a `http::Request`, opening a 
     // file. 
     virtual void Work() = 0;

     // Was the `Work` unresponsive, or did it time out.
     virtual bool TimedOut() = 0;

     // Is all work done.
     virtual bool Done() = 0;
  };
  // Create the `EventLoop`, the loop is bound to the current thread.
  // It also registers itself with the thread-local `EventLoop` stack.
  static EventLoop Create();

  // Return the current `EventLoop`.
  static EventLoop* Get();

  // Has this `EventLoop` received a `Quit`. If this is true, the Loop begins 
  // winding down.
  bool QuitNotified() const;

  // Run the Loop. 
  void Exec();
 private:
  // Incoming work, which is not done.
  absl::InlinedVector<Work,16> work_queue_;

};

}
#endif
