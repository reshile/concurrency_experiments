#pragma once

#include <twist/ed/std/mutex.hpp>
#include <twist/ed/std/condition_variable.hpp>
#include <deque>

#include <optional>

// Unbounded blocking multi-producers/multi-consumers (MPMC) queue

template <typename T>
class UnboundedBlockingQueue {
 public:
  void Push(T task) {
    std::lock_guard<twist::ed::std::mutex> lock(mutex_);

    if (!closed_) {
      storage_.push_back(std::move(task));
      is_empty_.notify_one();
    }
  }

  std::optional<T> Pop() {
    std::unique_lock<twist::ed::std::mutex> ulock(mutex_);

    while (!closed_ && storage_.empty()) {
      is_empty_.wait(ulock);
    }

    if (storage_.empty()) {
      return std::nullopt;
    }

    T obj = std::move(storage_.front());
    storage_.pop_front();

    return obj;
  }

  void Close() {
    std::unique_lock<twist::ed::std::mutex> ulock(mutex_);
    closed_ = true;
    is_empty_.notify_all();
  }

 private:
  std::deque<T> storage_;
  twist::ed::std::mutex mutex_;
  twist::ed::std::condition_variable is_empty_;
  bool closed_ = false;
};