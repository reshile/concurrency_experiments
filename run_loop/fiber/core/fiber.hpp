#pragma once

#include "body.hpp"
#include "coroutine.hpp"
#include "scheduler.hpp"

#include <twist/ed/static/thread_local/ptr.hpp>

namespace exe::fiber {

// Fiber = stackful coroutine + scheduler

class Fiber {
 public:
  Fiber(IScheduler&, Body);

  void ResumeCoro();

  void Suspend();  // remove after tests

  void FiberResume();

  bool IsCompleted();

  Coroutine* GetCoro();

  IScheduler& GetScheduler();

  static Fiber* Self();

 private:
  IScheduler* scheduler_;
  Coroutine coro_;
};

TWISTED_STATIC_THREAD_LOCAL_PTR(Fiber, current_fiber);

}  // namespace exe::fiber
