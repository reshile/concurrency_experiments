#pragma once

#include <twist/ed/std/condition_variable.hpp>
#include <twist/ed/std/atomic.hpp>
#include <twist/ed/wait/futex.hpp>
#include "twist/ed/std/mutex.hpp"
#include <cstddef>

namespace exe::thread {

class WaitGroup {
 public:
  void Add(size_t count) {
    std::unique_lock<twist::ed::std::mutex> ulock(mutex_);
    tasks_ += count;
  }

  void Done() {
    std::unique_lock<twist::ed::std::mutex> ulock(mutex_);
    // std::unique_lock<twist::ed::std::mutex> barrier(barrier_mutex_);

    if (--tasks_ == 0 && is_waiting_) {
      is_zero_.notify_all();
    }
  }

  void Wait() {
    std::unique_lock<twist::ed::std::mutex> ulock(mutex_);

    is_waiting_ = true;
    is_zero_.wait(ulock, [&] {
      return tasks_ == 0;
    });
    // barrier_mutex_.lock();
    is_waiting_ = false;
    // barrier_mutex_.unlock();
  }

 private:
  twist::ed::std::condition_variable is_zero_;
  twist::ed::std::mutex mutex_;
  // twist::ed::std::mutex barrier_mutex_;
  uint64_t tasks_ = 0;
  bool is_waiting_ = false;
};

}  // namespace exe::thread
