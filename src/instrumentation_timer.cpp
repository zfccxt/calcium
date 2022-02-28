#ifdef CALCIUM_BUILD_PROFILE

#include "instrumentation_timer.hpp"

#include <cstdint>
#include <thread>

#include "instrumentor.hpp"

namespace cl {

Timer::Timer(const char* name) : name_(name) {
  start_time_ = std::chrono::high_resolution_clock::now();
}

Timer::~Timer() {
  if (!stopped_) Stop();
}

void Timer::Stop() {
  auto end_time = std::chrono::high_resolution_clock::now();
  
  long long start = std::chrono::time_point_cast<std::chrono::microseconds>(start_time_).time_since_epoch().count();
  long long end   = std::chrono::time_point_cast<std::chrono::microseconds>(end_time   ).time_since_epoch().count();
  
  stopped_ = true;
  
  uint32_t thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());
  Instrumentor::WriteProfile(name_, start, end, thread_id);
}

}

#endif