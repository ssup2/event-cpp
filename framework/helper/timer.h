#ifndef TIMER_H_
#define TIMER_H_

#include <event/event.h>

class Timer {
 public:
  Timer(int sec, int inter);
  ~Timer();

  Event* GetEvent(std::string event_name);

  int SetTimer(int sec, int inter);
  int Clear();

 private:
  int timer_fd_;
};

#endif  // TIMER_H_
