#include <common/read_write.h>

#include "signal.h"

Signal::Signal(std::string event_name) {
  sigemptyset(&mask_);

  event_name_ = event_name;
  event_.release();
}

Signal::~Signal() {}

Event* Signal::GetEvent() { return event_.get(); }

bool Signal::AddSignal(int signal) {
  sigaddset(&mask_, signal);
  if (sigprocmask(SIG_BLOCK, &mask_, nullptr) == -1) return false;

  return true;
}

bool Signal::SetSignal() {
  signal_fd_ = signalfd(-1, &mask_, SFD_NONBLOCK);
  if (signal_fd_ == -1) return false;

  event_.reset(new Event(signal_fd_, event_name_));
  return true;
}

int Signal::GetSignal() {
  signalfd_siginfo sig_info;

  if (ReadWrite::ReadAll(signal_fd_, &sig_info, sizeof(signalfd_siginfo)) == -1)
    return -1;

  return sig_info.ssi_signo;
}
