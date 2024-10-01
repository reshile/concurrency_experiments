#include "yield.hpp"
#include "../core/fiber.hpp"

namespace exe::fiber {

void Yield() {
  // auto fib = Fiber::Self();
  Fiber::Self()->GetCoro()->Suspend();
}

}  // namespace exe::fiber
