#include "sleep_for.hpp"

#include "exe/fiber/core/fiber.hpp"
#include <asio/steady_timer.hpp>

namespace exe::fiber {

void SleepFor(std::chrono::milliseconds delay) {
  asio::steady_timer timer(Fiber::Self()->GetScheduler());

  Fiber::Self()->SetHandler(
      [&, delay = std::move(delay)](auto current) {
        timer.expires_after(delay);
        timer.async_wait([current = std::move(current)](std::error_code) {
          current->Run();
        });
      },
      States::Sleeping);

  Fiber::Self()->GetCoro()->Suspend();
}

}  // namespace exe::fiber
