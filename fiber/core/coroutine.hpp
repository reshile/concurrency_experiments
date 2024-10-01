#pragma once

#include "stack.hpp"

#include <sure/context.hpp>

#include <function2/function2.hpp>

namespace exe::fiber {

class Coroutine : private sure::ITrampoline {
 public:
  class SuspendContext {
    friend class Coroutine;

   public:
    void Suspend() {
      coro_->Suspend();
    }

   private:
    explicit SuspendContext(Coroutine* coro)
        : coro_(coro) {
    }

   private:
    Coroutine* coro_;
  };

  using Body = fu2::unique_function<void(SuspendContext)>;

 public:
  explicit Coroutine(Body, size_t memory_size = 4096 * 3);

  void Resume();

  bool IsCompleted() const;

  void Suspend();

  SuspendContext GetSuspend() {
    return SuspendContext(this);
  }

 private:
  [[noreturn]] void Run() noexcept override;

 private:
  Stack stack_;
  Body user_lamda_;
  sure::ExecutionContext caller_context_;
  sure::ExecutionContext context_;
  bool completed_ = false;
};

}  // namespace exe::fiber
