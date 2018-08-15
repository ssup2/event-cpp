#ifndef EVENT_H_
#define EVENT_H_

#include <string>

class Event {
 private:
  int id_;  // fd number
  std::string name_;

 public:
  Event(int id, std::string name) {
    id_ = id;
    name_ = name;
  }
  ~Event() {}

  int GetId() { return id_; }
  std::string GetName() { return name_; }
};

#endif  // EVENT_H_
