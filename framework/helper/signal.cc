#include <common/read_write.h>

#include "signal.h"

Signal::Signal() { sigemptyset(&mask_); }

Signal::~Signal() {}

Event* Signal::GetEvent(std::string event_name) {
  return new Event(signal_fd_, event_name);
}

int Signal::AddSignal(int signal) {
  sigaddset(&mask_, signal);
  if (sigprocmask(SIG_BLOCK, &mask_, nullptr) == -1) return -1;

  return 0;
}

int Signal::SetSignal() {
  signal_fd_ = signalfd(-1, &mask_, 0);
  if (signal_fd_ == -1) return -1;

  return 0;
}

int Signal::GetSignal() {
  signalfd_siginfo sig_info;

  if (ReadWrite::ReadAll(signal_fd_, &sig_info, sizeof(signalfd_siginfo)) == -1)
    return -1;

  return sig_info.ssi_signo;
}
