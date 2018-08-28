#ifndef SIGNAL_H_
#define SIGNAL_H_

#include <csignal>
#include <memory>

#include <event/event.h>
#include <sys/signalfd.h>

class Signal {
 public:
  Signal(std::string event_name);
  ~Signal();

  Event* GetEvent();

  bool AddSignal(int signal);
  bool SetSignal();
  int GetSignal();

 private:
  std::unique_ptr<Event> event_;
  std::string event_name_;

  int signal_fd_;
  sigset_t mask_;
};

#endif  // SIGNAL_H_
