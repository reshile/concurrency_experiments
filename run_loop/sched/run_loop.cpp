#include "run_loop.hpp"

namespace exe::sched {

void RunLoop::Submit(task::Task task) {
  tasks_.push(std::move(task));
}

// Run tasks

size_t RunLoop::RunAtMost(size_t limit) {
  size_t execute_count = std::min(limit, tasks_.size());
  for (size_t i = 0; i < execute_count; ++i) {
    auto task = std::move(tasks_.front());
    tasks_.pop();
    task();
  }
  return execute_count;
}

size_t RunLoop::Run() {
  size_t execute_count = 0;
  while (!tasks_.empty()) {
    execute_count += RunAtMost(tasks_.size());
  }
  return execute_count;
}

}  // namespace exe::sched
