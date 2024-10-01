#pragma once

#include "body.hpp"
#include "coroutine.hpp"
#include "scheduler.hpp"
#include "handler.hpp"

namespace exe::fiber {

enum States : uint32_t {
  Completed = 0,
  Yielded = 1,
  Sleeping = 2,
};

// Fiber = stackful coroutine + scheduler

class Fiber {
 public:
  Fiber(Scheduler&, Body);

  void ResumeCoro();

  void Suspend();  // remove after tests

  void FiberResume();

  void Run();

  bool IsCompleted();

  Coroutine* GetCoro();

  Scheduler& GetScheduler();

  static Fiber* Self();

  void Sleep();

  void SetHandler(Handler, uint32_t);

  void CallHandler();

  void Suspend(Handler);

  uint32_t GetState() {
    return state_;
  }

 private:
  Scheduler* scheduler_;
  Coroutine coro_;
  Handler handler_;
  uint32_t state_;
};

}  // namespace exe::fiber
