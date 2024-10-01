#include "coroutine.hpp"

namespace exe::fiber {

Coroutine::Coroutine(Body user_lamda, size_t memory_size)
    : memory_size_(memory_size),
      stack_(Stack::AllocateBytes(memory_size_)),
      user_lamda_(std::move(user_lamda)) {
  coro_context_.Setup(stack_.MutView(), this);
}

void Coroutine::Resume() {
  main_context_.SwitchTo(coro_context_);
}

void Coroutine::Suspend() {
  coro_context_.SwitchTo(main_context_);
}

void Coroutine::Run() noexcept {
  try {
    user_lamda_(SuspendContext(this));
  } catch (...) {
    // fmt::println("Something bad happened");
  }

  completed_ = true;
  coro_context_.ExitTo(main_context_);
}

bool Coroutine::IsCompleted() const {
  return completed_;
}

}  // namespace exe::fiber
