#ifndef MANAGER_H_
#define MANAGER_H_

#include <map>
#include <memory>
#include <vector>

#include "event.h"
#include "handler.h"

class EventInfo {
 private:
  int target_handler_;
  std::unique_ptr<Event> event_;

 public:
  EventInfo(int target_handler, Event* event) {
    target_handler_ = target_handler;
    event_.reset(event);
  }
  ~EventInfo() {}

  int GetTargetHandler() { return target_handler_; }
  Event* GetEvent() { return event_.get(); }
};

class HandlerInfo {
 public:
  enum Return { kSucc = 0, kFail = -1 };

 private:
  std::unique_ptr<EventHandler> handler_;
  std::unique_ptr<HandlerContext> context_;
  std::map<std::string, int> event_map_;

 public:
  HandlerInfo(EventHandler* handler, HandlerContext* context)
      : handler_(handler), context_(context) {}
  ~HandlerInfo() {}

  EventHandler* GetHandler() { return handler_.get(); }
  HandlerContext* GetContext() { return context_.get(); }

  int GetEventId(std::string event_name) {
    if (event_map_.find(event_name) == event_map_.end()) return Return::kFail;
    return event_map_[event_name];
  }
  void SetEventId(std::string event_name, int event_id) {
    event_map_[event_name] = event_id;
  }
  void UnsetEventId(std::string event_name) {
    if (event_map_.find(event_name) == event_map_.end()) return;
    event_map_.erase(event_map_.find(event_name));
  }
};

class EventManager {
 public:
  static const int kMaxEventCount = 1024;
  static const int kMaxHandlerCount = 512;
  static const int kStdEventCount = 3;  // 3 is for std in,out,err

 private:
  int epoll_fd_;
  int handler_count_;
  int handler_next_id_;

  std::map<std::string, int> handler_map_;
  std::unique_ptr<HandlerInfo> handler_infos_[kMaxHandlerCount];
  std::unique_ptr<EventInfo> event_infos_[kMaxEventCount + kStdEventCount];

  EventManager();
  ~EventManager();

 public:
  static EventManager* GetManager();
  bool Run();

  int GetHandlerId(std::string handler_name);
  bool AddHandler(std::string handler_name, EventHandler* handler);
  int DelHandler(std::string handler_name);

  int GetEventId(std::string handler_name, std::string event_name);
  bool AddEvent(std::string handler_name, Event* event);
  bool DelEvent(std::string handler_name, std::string event_name);

  bool SendKnock(std::string handler_name, std::string knock_event_name);
  bool PushObj(std::string handler_name, HandlerObj* obj);
  bool PushObj(std::string handler_name, std::string queue_name,
               HandlerObj* obj);
};

#endif  // MANAGER_H_
