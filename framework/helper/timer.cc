#include <unistd.h>
#include <cstdint>
#include <ctime>

#include <common/read_write.h>
#include <sys/timerfd.h>

#include "timer.h"

Timer::Timer(int sec, int inter) {
  timer_fd_ = timerfd_create(CLOCK_MONOTONIC, 0);
  if (timer_fd_ == -1) return;

  struct itimerspec interval;
  interval.it_value.tv_sec = sec;
  interval.it_value.tv_nsec = 0;
  interval.it_interval.tv_sec = inter;
  interval.it_interval.tv_nsec = 0;

  if (timerfd_settime(timer_fd_, 0, &interval, NULL) < 0) {
    close(timer_fd_);
    timer_fd_ = -1;
  }
}

Timer::~Timer() {
  if (timer_fd_ == -1) close(timer_fd_);
}

Event* Timer::GetEvent(std::string event_name) {
  return new Event(timer_fd_, event_name);
}

int Timer::SetTimer(int sec, int inter) {
  struct itimerspec interval;
  interval.it_value.tv_sec = sec;
  interval.it_value.tv_nsec = 0;
  interval.it_interval.tv_sec = inter;
  interval.it_interval.tv_nsec = 0;

  if (timerfd_settime(timer_fd_, 0, &interval, NULL) < 0) return -1;

  return 0;
}

int Timer::Clear() {
  uint64_t expire_count;

  if (ReadWrite::ReadAll(timer_fd_, &expire_count, sizeof(uint64_t)) !=
      sizeof(uint64_t))
    return -1;

  return 0;
}
