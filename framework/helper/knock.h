#ifndef KNOCK_H_
#define KNOCK_H_

#include <event/event.h>
#include <memory>

class Knock {
 public:
  enum Mode { kNormal, kAggre };

 public:
  Knock(std::string event_name, Mode mode);
  ~Knock();

  Event* GetEvent();

  bool Send();
  bool Clear();

 private:
  std::unique_ptr<Event> event_;
  std::string event_name_;

  int event_fd_;
};

#endif  // KNOCK_H_
