#include <time.h>
#include <unistd.h>
#include <memory>

#include <logger/logger.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/timerfd.h>

#include "manager.h"

EventManager::EventManager() {
  handler_count_ = 0;
  handler_last_id_ = kMaxHandlerCount - 1;
  epoll_fd_ = epoll_create(1);
}

EventManager::~EventManager() { close(epoll_fd_); }

EventManager* EventManager::GetInstance() {
  static EventManager event_manager;
  return &event_manager;
}

int EventManager::Init() {
  for (int i = 0; i < handler_count_; i++)
    handler_infos_[i]->GetHandler()->Init(handler_infos_[i]->GetContext());

  return Return::kSucc;
}

int EventManager::Run() {
  Logger(Logger::kInfo) << "Run the event loop";

  while (true) {
    epoll_event events[kMaxEventCount];

    // Get events
    int count = epoll_wait(epoll_fd_, events, kMaxEventCount, -1);
    if (count == -1) {
      Logger(Logger::kCrit) << "epoll_wait() error";
      exit(EXIT_FAILURE);
    }

    // Process events
    for (int i = 0; i < count; i++) {
      int fd = events[i].data.fd;  // fd = eventid
      Event* event = event_infos_[fd]->GetEvent();

      int handler_id = event_infos_[fd]->GetTargetHandler();
      HandlerContext* handler_context =
          handler_infos_[handler_id]->GetContext();
      std::string handler_name = handler_context->GetHandlerName();

      // Run the handler
      Logger(Logger::kInfo) << "Handler Start - " << handler_name;
      handler_infos_[handler_id]->GetHandler()->Handler(handler_context, event);
      Logger(Logger::kInfo) << "Handler End - " << handler_name;
    }
  }

  return Return::kSucc;
}

int EventManager::GetHandlerId(std::string handler_name) {
  if (handler_map_.find(handler_name) == handler_map_.end()) {
    return Return::kFail;
  }
  return handler_map_[handler_name];
}

int EventManager::AddHandler(std::string handler_name, EventHandler* handler) {
  Logger(Logger::kInfo) << "Add a event handler - " << handler_name;

  if (handler_count_ > kMaxHandlerCount) {
    Logger(Logger::kWarn) << "Over the max event handler count - "
                          << handler_name;
    return Return::kFail;
  }

  if (GetHandlerId(handler_name) != Return::kFail) {
    Logger(Logger::kWarn) << "Duplicate handler - " << handler_name;
    return Return::kFail;
  }

  // Find a unused handler id
  while (handler_infos_[++handler_last_id_ % kMaxHandlerCount].get() ==
         nullptr) {
  }

  // Set handler map and handler info
  handler_map_[handler_name] = handler_last_id_;
  handler_infos_[handler_last_id_].reset(new HandlerInfo(
      handler, new HandlerContext(handler_last_id_, handler_name)));

  handler_count_++;
  return handler_last_id_;
}

int EventManager::DelHandler(std::string handler_name) {
  Logger(Logger::kInfo) << "Delete a event handler - " << handler_name;

  int handler_id;
  if ((handler_id = GetHandlerId(handler_name)) == Return::kFail) {
    Logger(Logger::kWarn) << "No handler - " << handler_name;
    return Return::kFail;
  }

  handler_map_.erase(handler_map_.find(handler_name));
  handler_infos_[handler_id].release();
  handler_count_--;
  return Return::kSucc;
}

int EventManager::GetEventId(std::string handler_name, std::string event_name) {
  int handler_id;
  if ((handler_id = GetHandlerId(handler_name)) == Return::kFail) {
    Logger(Logger::kWarn) << "No handler - " << handler_name;
    return Return::kFail;
  }

  int event_id;
  if ((event_id = handler_infos_[handler_id]->GetEventId(event_name)) ==
      HandlerInfo::Return::kFail) {
    return Return::kFail;
  }

  return event_id;
}

int EventManager::AddEvent(std::string handler_name, Event* event) {
  std::string log_name = handler_name + " - " + event->GetName();
  Logger(Logger::kInfo) << "Add a event - " << log_name;

  if (event->GetId() > kMaxEventCount + kStdEventCount) {
    Logger(Logger::kWarn) << "Over the max event count - " << log_name;
    return Return::kFail;
  }

  int handler_id;
  if ((handler_id = GetHandlerId(handler_name)) == Return::kFail) {
    Logger(Logger::kWarn) << "No handler - " << log_name;
    return Return::kFail;
  }

  int event_id;
  if ((event_id = GetEventId(handler_name, event->GetName())) !=
      Return::kFail) {
    Logger(Logger::kWarn) << "Duplicate event - " << log_name;
    return Return::kFail;
  }

  // Add event to epoll
  epoll_event ep_event;
  ep_event.data.fd = event->GetId();
  ep_event.events = EPOLLIN;
  epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, event->GetId(), &ep_event);

  // Set infos
  event_infos_[event->GetId()].reset(new EventInfo(handler_id, event));
  handler_infos_[handler_id]->SetEventId(event->GetName(), event->GetId());
  return Return::kSucc;
}

int EventManager::DelEvent(std::string handler_name, std::string event_name) {
  std::string log_name = handler_name + " - " + event_name;
  Logger(Logger::kInfo) << "Delete a event - " << log_name;

  int handler_id;
  if ((handler_id = GetHandlerId(handler_name)) == Return::kFail) {
    Logger(Logger::kWarn) << "No handler - " << log_name;
    return Return::kFail;
  }

  int event_id;
  if ((event_id = GetEventId(handler_name, event_name)) == Return::kFail) {
    Logger(Logger::kWarn) << "No event - " << log_name;
    return Return::kFail;
  }

  // Delete event from epoll
  epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, event_id, nullptr);

  // Unset infos
  event_infos_[event_id].release();
  handler_infos_[handler_id]->UnsetEventId(event_name);
  return Return::kSucc;
}

int EventManager::SendKnock(std::string handler_name,
                            std::string knock_event_name) {
  std::string log_name = handler_name + " - " + knock_event_name;

  int handler_id;
  if ((handler_id = GetHandlerId(handler_name)) == Return::kFail) {
    Logger(Logger::kWarn) << "No handler - " << log_name;
    return Return::kFail;
  }

  int event_id;
  if ((event_id = GetEventId(handler_name, knock_event_name)) ==
      Return::kFail) {
    Logger(Logger::kWarn) << "No event - " << log_name;
    return Return::kFail;
  }

  // Write to eventfd
  if (eventfd_write(event_id, 1) != 0) return Return::kFail;

  return Return::kSucc;
}

int EventManager::PushObj(std::string handler_name, std::string queue_name,
                          HandlerObj* obj) {
  std::string log_name = handler_name + " - " + queue_name;

  int handler_id;
  if ((handler_id = GetHandlerId(handler_name)) == Return::kFail) {
    Logger(Logger::kWarn) << "No handler - " << log_name;
    return Return::kFail;
  }

  // Push object
  handler_infos_[handler_id]->GetContext()->PushQueue(
      queue_name, std::unique_ptr<HandlerObj>(obj));
  return Return::kSucc;
}

int EventManager::PushObj(std::string handler_name, HandlerObj* obj) {
  return PushObj(handler_name, HandlerContext::kDefaultQueueName, obj);
}
