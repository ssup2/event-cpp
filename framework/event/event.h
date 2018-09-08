#ifndef EVENT_H_
#define EVENT_H_

#include <string>

class Event {
 public:
  enum Type {
    kKnock,
    kSignal,
    kTimer,
    kTcpConn,
    kUser,
  };

 private:
  int id_;  // fd number
  std::string name_;
  Type type_;

 public:
  Event(int id, std::string name) {
    id_ = id;
    name_ = name;
    type_ = kUser;
  }
  Event(int id, std::string name, Type type) {
    id_ = id;
    name_ = name;
    type_ = type;
  }
  ~Event() {}

  int GetId() { return id_; }
  std::string GetName() { return name_; }
  int GetType() { return type_; }
};

#endif  // EVENT_H_
