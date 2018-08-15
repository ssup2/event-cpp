#ifndef KNOCK_H_
#define KNOCK_H_

#include <event/event.h>

class Knock {
 public:
  enum Mode { kNormal, kAggre };

 public:
  Knock(Mode mode);
  ~Knock();

  Event* GetEvent(std::string event_name);

  int Send();
  int Clear();

 private:
  int event_fd_;
};

#endif  // KNOCK_H_
