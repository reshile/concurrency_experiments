#include "fiber.hpp"

#include <cstdlib>

namespace exe::fiber {

Fiber::Fiber(IScheduler& scheduler, Body user_lambda)
    : scheduler_(&scheduler),
      coro_([user_lambda = std::move(user_lambda)](auto) mutable {
        user_lambda();
      }) {
}

void Fiber::ResumeCoro() {
  coro_.Resume();
}

void Fiber::Suspend() {  // remove after tests
  coro_.GetSuspend().Suspend();
}

void Fiber::FiberResume() {
  auto f = [this] {
    current_fiber = this;
    coro_.Resume();
    if (coro_.IsCompleted()) {
      delete current_fiber;
    } else {
      FiberResume();
    }
  };
  scheduler_->Submit(f);
}

Coroutine* Fiber::GetCoro() {
  return &coro_;
}

IScheduler& Fiber::GetScheduler() {
  return *scheduler_;
}

bool Fiber::IsCompleted() {
  return coro_.IsCompleted();
}

Fiber* Fiber::Self() {
  return exe::fiber::current_fiber;
}
}  // namespace exe::fiber
