#include "go.hpp"

#include "../core/fiber.hpp"

namespace exe::fiber {

void Go(Scheduler& shced, Body user_lambda) {
  auto fib = new Fiber(shced, std::move(user_lambda));
  fib->FiberResume();
}

void Go(Body user_lambda) {
  Go(Fiber::Self()->GetScheduler(), std::move(user_lambda));
}

}  // namespace exe::fiber
