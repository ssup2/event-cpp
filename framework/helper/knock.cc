#include <unistd.h>
#include <iostream>

#include <sys/eventfd.h>

#include "knock.h"

Knock::Knock(Mode mode) {
  if (mode == kAggre) {
    event_fd_ = eventfd(0, EFD_SEMAPHORE);
  } else {
    event_fd_ = eventfd(0, 0);
  }
}

Knock::~Knock() { close(event_fd_); }

Event* Knock::GetEvent(std::string event_name) {
  return new Event(event_fd_, event_name);
}

int Knock::Send() {
  if (eventfd_write(event_fd_, 1) != 0) {
    return -1;
  }
  return 0;
}

int Knock::Clear() {
  eventfd_t tmp;
  if (eventfd_read(event_fd_, &tmp) != 0) {
    return -1;
  }
  return 0;
}
