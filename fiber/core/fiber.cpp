#include "fiber.hpp"

#include <cstdlib>
#include <twist/ed/static/thread_local/ptr.hpp>

namespace exe::fiber {

TWISTED_STATIC_THREAD_LOCAL_PTR(Fiber, current_fiber);

Fiber::Fiber(Scheduler& scheduler, Body user_lambda)
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

void Fiber::Run() {
  current_fiber = this;
  coro_.Resume();
  if (IsCompleted()) {
    delete current_fiber;
    // CallHandler();
  } else {
    CallHandler();
  }
}

void Fiber::FiberResume() {
  auto f = [this] {
    this->Run();
  };
  scheduler_->post(f);  // fix later
}

Coroutine* Fiber::GetCoro() {
  return &coro_;
}

Scheduler& Fiber::GetScheduler() {
  return *scheduler_;
}

bool Fiber::IsCompleted() {
  return coro_.IsCompleted();
}

void Fiber::SetHandler(Handler func, uint32_t state) {
  state_ = state;
  handler_ = std::move(func);
}

void Fiber::CallHandler() {
  handler_(this);
}

void Fiber::Sleep() {
  handler_(this);
}

Fiber* Fiber::Self() {
  return exe::fiber::current_fiber;
}
}  // namespace exe::fiber
