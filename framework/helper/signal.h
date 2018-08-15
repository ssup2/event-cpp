#ifndef SIGNAL_H_
#define SIGNAL_H_

#include <csignal>

#include <event/event.h>
#include <sys/signalfd.h>

class Signal {
 public:
  Signal();
  ~Signal();

  Event* GetEvent(std::string event_name);

  int AddSignal(int signal);
  int SetSignal();

  int GetSignal();

 private:
  int signal_fd_;
  sigset_t mask_;
};

#endif  // SIGNAL_H_
