#pragma once

#include <function2/function2.hpp>

namespace exe::sched::task {

using Task = fu2::unique_function<void()>;

}  // namespace exe::sched::task
