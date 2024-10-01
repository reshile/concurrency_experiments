#include "go.hpp"
#include "../core/fiber.hpp"

namespace exe::fiber {

void Go(IScheduler& shced, Body user_lambda) {
  auto fib = new Fiber(shced, std::move(user_lambda));
  fib->FiberResume();  // submit resume
}

void Go(Body user_lambda) {
  auto fib = new Fiber(Fiber::Self()->GetScheduler(), std::move(user_lambda));
  fib->FiberResume();
}

}  // namespace exe::fiber
