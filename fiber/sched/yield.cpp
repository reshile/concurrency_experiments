#include "yield.hpp"
#include "../core/fiber.hpp"

namespace exe::fiber {

void Yield() {
  Fiber::Self()->SetHandler(
      [](auto current) {
        current->FiberResume();
      },
      States::Yielded);
  Fiber::Self()->GetCoro()->Suspend();
}

}  // namespace exe::fiber
