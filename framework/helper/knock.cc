#include <unistd.h>
#include <iostream>

#include <sys/eventfd.h>

#include "knock.h"

Knock::Knock(std::string event_name, Mode mode) {
  if (mode == kAggre) {
    event_fd_ = eventfd(0, EFD_SEMAPHORE);
  } else {
    event_fd_ = eventfd(0, 0);
  }

  event_name_ = event_name;
  event_.reset(new Event(event_fd_, event_name));
}

Knock::~Knock() { close(event_fd_); }

Event* Knock::GetEvent() { return event_.get(); }

bool Knock::Send() {
  if (eventfd_write(event_fd_, 1) != 0) {
    return false;
  }
  return true;
}

bool Knock::Clear() {
  eventfd_t tmp;
  if (eventfd_read(event_fd_, &tmp) != 0) {
    return false;
  }
  return true;
}
