#ifndef TIMER_H_
#define TIMER_H_

#include <event/event.h>
#include <memory>

class Timer {
 public:
  Timer(std::string event_name);
  ~Timer();

  Event* GetEvent();

  bool SetTimer(int init_msec, int inter_msec);
  bool Clear();

 private:
  std::unique_ptr<Event> event_;
  std::string event_name_;

  int timer_fd_;
};

#endif  // TIMER_H_
