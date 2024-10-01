#include "go.hpp"
#include "../core/fiber.hpp"

#include <cassert>

namespace exe::fiber {

void Go(Scheduler& shced, Body user_lambda) {
  auto fib = new Fiber(shced, std::move(user_lambda));
  fib->FiberResume();  // submit resume
}

void Go(Body user_lambda) {
  auto fib = new Fiber(Fiber::Self()->GetScheduler(), std::move(user_lambda));
  fib->FiberResume();
}

}  // namespace exe::fiber
