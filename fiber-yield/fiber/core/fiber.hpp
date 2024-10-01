#pragma once

#include "body.hpp"
#include "coroutine.hpp"
#include "scheduler.hpp"

#include <twist/ed/static/thread_local/ptr.hpp>

namespace exe::fiber {

// Fiber = stackful coroutine + scheduler

class alignas(256) Fiber {
 public:
  Fiber(Scheduler&, Body);

  void ResumeCoro();

  void Suspend();  // remove after tests

  void FiberResume();

  bool IsCompleted();

  Coroutine* GetCoro();

  Scheduler& GetScheduler();

  static Fiber* Self();

 private:
  Scheduler* scheduler_;
  Coroutine coro_;
};

TWISTED_STATIC_THREAD_LOCAL_PTR(Fiber, current_fiber);

}  // namespace exe::fiber
