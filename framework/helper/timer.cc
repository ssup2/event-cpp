#include <unistd.h>
#include <cstdint>
#include <ctime>

#include <common/read_write.h>
#include <sys/timerfd.h>

#include "timer.h"

Timer::Timer(std::string event_name) {
  timer_fd_ = timerfd_create(CLOCK_MONOTONIC, 0);
  if (timer_fd_ == -1) return;

  event_name_ = event_name;
  event_.reset(new Event(timer_fd_, event_name, Event::Type::kTimer));
}

Timer::~Timer() {
  if (timer_fd_ == -1) close(timer_fd_);
}

Event* Timer::GetEvent() { return event_.get(); }

bool Timer::SetTimer(int init_msec, int inter_msec) {
  struct itimerspec time;
  time.it_value.tv_sec = init_msec / 1000;
  time.it_value.tv_nsec = (init_msec % 1000) * 1000000;
  time.it_interval.tv_sec = inter_msec / 1000;
  time.it_interval.tv_nsec = (inter_msec % 1000) * 1000000;

  if (timerfd_settime(timer_fd_, 0, &time, NULL) < 0) return false;

  return true;
}

bool Timer::Clear() {
  uint64_t expire_count;

  if (ReadWrite::ReadAll(timer_fd_, &expire_count, sizeof(uint64_t)) !=
      sizeof(uint64_t))
    return false;

  return true;
}
