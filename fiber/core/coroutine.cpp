#include "coroutine.hpp"
#include "fmt/core.h"

namespace exe::fiber {

Coroutine::Coroutine(Body user_lamda, size_t memory_size)
    : stack_(Stack::AllocateBytes(memory_size)),
      user_lamda_(std::move(user_lamda)) {
  context_.Setup(stack_.MutView(), this);
}

void Coroutine::Resume() {
  caller_context_.SwitchTo(context_);
}

void Coroutine::Suspend() {
  context_.SwitchTo(caller_context_);
}

void Coroutine::Run() noexcept {
  try {
    user_lamda_(SuspendContext(this));
  } catch (...) {
    fmt::println("Something bad happened.");
  }

  completed_ = true;
  context_.ExitTo(caller_context_);
}

bool Coroutine::IsCompleted() const {
  return completed_;
}

}  // namespace exe::fiber