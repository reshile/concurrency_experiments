#pragma once

#include "queue.hpp"
#include "exe/sched/task/task.hpp"

#include <twist/ed/std/thread.hpp>
#include <vector>

#include <exe/sched/task/scheduler.hpp>

#include <cstddef>

namespace exe::sched {
class ThreadPool : public task::IScheduler {
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

  // task::IScheduler
  void Submit(task::Task) override;

  static ThreadPool* Current();

  void Stop();

 private:
  void WorkRoutine();

  ThreadPool* CurrentHelper();

  UnboundedBlockingQueue<task::Task> tasks_;
  std::vector<twist::ed::std::thread> threads_;
  const size_t thread_cnt_;
  bool stop_called_ = false;
};

}  // namespace exe::sched
