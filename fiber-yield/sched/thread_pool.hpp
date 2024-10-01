#pragma once

#include "queue.hpp"
#include "task.hpp"

#include <twist/ed/std/thread.hpp>
#include <vector>

#include <cstddef>

namespace exe::sched {

class ThreadPool {
 public:
  explicit ThreadPool(size_t threads);

  ~ThreadPool();

  // Non-copyable
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

  // Non-movable
  ThreadPool(ThreadPool&&) = delete;
  ThreadPool& operator=(ThreadPool&&) = delete;

  void Start();

  void Submit(Task);

  static ThreadPool* Current();

  void Stop();

 private:
  void WorkRoutine();

  ThreadPool* CurrentHelper();

  UnboundedBlockingQueue<Task> tasks_;
  std::vector<twist::ed::std::thread> threads_;
  const size_t thread_cnt_;
  bool stop_called_ = false;
};

}  // namespace exe::sched
