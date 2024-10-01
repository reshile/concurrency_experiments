#include "thread_pool.hpp"

#include <twist/ed/static/thread_local/ptr.hpp>

#include <wheels/core/panic.hpp>

namespace exe::sched {

TWISTED_STATIC_THREAD_LOCAL_PTR(ThreadPool, worker_access);

ThreadPool::ThreadPool(size_t threads)
    : thread_cnt_(threads) {
}

void ThreadPool::Start() {
  for (size_t i = 0; i < thread_cnt_; ++i) {
    threads_.emplace_back([this] {
      worker_access = this;
      WorkRoutine();
    });
  }
}

ThreadPool::~ThreadPool() {
  assert(stop_called_);
}

void ThreadPool::Submit(Task task) {
  tasks_.Push(std::move(task));
}

ThreadPool* ThreadPool::CurrentHelper() {
  return this;
}

ThreadPool* ThreadPool::Current() {
  return worker_access;
}

void ThreadPool::Stop() {
  tasks_.Close();
  stop_called_ = true;
  for (size_t i = 0; i < thread_cnt_; ++i) {
    threads_[i].join();
  }
}

void ThreadPool::WorkRoutine() {
  while (true) {
    auto task = tasks_.Pop();
    if (!task.has_value()) {
      break;
    }
    (*task)();
  }
}

}  // namespace exe::sched
