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
  handler_next_id_ = 0;
  epoll_fd_ = epoll_create(1);
}

EventManager::~EventManager() { close(epoll_fd_); }

EventManager* EventManager::GetManager() {
  static EventManager event_manager;
  return &event_manager;
}

bool EventManager::Run() {
  Logger(Logger::kInfo) << "EventManager - Run the event loop";

  while (true) {
    epoll_event events[kMaxEventCount];

    // Get events
    int count = epoll_wait(epoll_fd_, events, kMaxEventCount, -1);
    if (count == -1) {
      Logger(Logger::kCrit) << "EventManager - epoll_wait() error";
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
      Logger(Logger::kInfo)
          << "EventManager - Handler Start - " << handler_name;
      handler_infos_[handler_id]->GetHandler()->Handler(handler_context, event);
      Logger(Logger::kInfo) << "EventManager - Handler End - " << handler_name;
    }
  }

  return true;
}

int EventManager::GetHandlerId(std::string handler_name) {
  if (handler_map_.find(handler_name) == handler_map_.end()) {
    return -1;
  }
  return handler_map_[handler_name];
}

bool EventManager::AddHandler(std::string handler_name, EventHandler* handler) {
  Logger(Logger::kInfo) << "EventManager - Add a event handler - "
                        << handler_name;

  if (handler_count_ > kMaxHandlerCount) {
    Logger(Logger::kWarn)
        << "EventManager - Over the max event handler count - " << handler_name;
    return false;
  }

  if (GetHandlerId(handler_name) != -1) {
    Logger(Logger::kWarn) << "EventManager - Duplicate handler - "
                          << handler_name;
    return false;
  }

  // Find a unused handler id
  while (handler_infos_[handler_next_id_].get() != nullptr) {
    handler_next_id_ = (handler_next_id_ + 1) % kMaxHandlerCount;
  }

  // Set handler map and handler info
  handler_map_[handler_name] = handler_next_id_;
  handler_infos_[handler_next_id_].reset(new HandlerInfo(
      handler, new HandlerContext(handler_next_id_, handler_name)));

  // Init handler
  if (!handler->Init(handler_infos_[handler_next_id_]->GetContext())) {
    handler_infos_[handler_next_id_].release();
    Logger(Logger::kWarn) << "EventManager - Init handler false - "
                          << handler_name;
    return false;
  }

  Logger(Logger::kInfo) << "EventManager - Added Handler ID - "
                        << handler_next_id_;
  handler_count_++;
  handler_next_id_++;

  return true;
}

int EventManager::DelHandler(std::string handler_name) {
  Logger(Logger::kInfo) << "EventManager - Delete a event handler - "
                        << handler_name;

  int handler_id;
  if ((handler_id = GetHandlerId(handler_name)) == -1) {
    Logger(Logger::kWarn) << "EventManager - No handler - " << handler_name;
    return false;
  }

  handler_map_.erase(handler_map_.find(handler_name));
  handler_infos_[handler_id].release();
  handler_count_--;
  return false;
}

int EventManager::GetEventId(std::string handler_name, std::string event_name) {
  int handler_id;
  if ((handler_id = GetHandlerId(handler_name)) == -1) {
    Logger(Logger::kWarn) << "EventManager - No handler - " << handler_name;
    return -1;
  }

  int event_id;
  if ((event_id = handler_infos_[handler_id]->GetEventId(event_name)) ==
      HandlerInfo::Return::kFail) {
    return -1;
  }

  return event_id;
}

bool EventManager::AddEvent(std::string handler_name, Event* event) {
  std::string log_name = handler_name + " - " + event->GetName();
  Logger(Logger::kInfo) << "EventManager - Add a event - " << log_name;

  if (event->GetId() > kMaxEventCount + kStdEventCount) {
    Logger(Logger::kWarn) << "EventManager - Over the max event count - "
                          << log_name;
    return true;
  }

  int handler_id;
  if ((handler_id = GetHandlerId(handler_name)) == -1) {
    Logger(Logger::kWarn) << "EventManager - No handler - " << log_name;
    return false;
  }

  int event_id;
  if ((event_id = GetEventId(handler_name, event->GetName())) != -1) {
    Logger(Logger::kWarn) << "EventManager - Duplicate event - " << log_name;
    return false;
  }

  // Add event to epoll
  epoll_event ep_event;
  ep_event.data.fd = event->GetId();
  ep_event.events = EPOLLIN;
  epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, event->GetId(), &ep_event);

  // Set infos
  event_infos_[event->GetId()].reset(new EventInfo(handler_id, event));
  handler_infos_[handler_id]->SetEventId(event->GetName(), event->GetId());

  // If event type is knock, Regist it to knock_map_ to broadcast knock.
  if (event->GetType() == Event::Type::kKnock) {
    knock_map_[event->GetName()].push_back(event->GetId());
  }

  return true;
}

bool EventManager::DelEvent(std::string handler_name, std::string event_name) {
  std::string log_name = handler_name + " - " + event_name;
  Logger(Logger::kInfo) << "EventManager - Delete a event - " << log_name;

  int handler_id;
  if ((handler_id = GetHandlerId(handler_name)) == -1) {
    Logger(Logger::kWarn) << "EventManager - No handler - " << log_name;
    return false;
  }

  int event_id;
  if ((event_id = GetEventId(handler_name, event_name)) == -1) {
    Logger(Logger::kWarn) << "EventManager - No event - " << log_name;
    return false;
  }

  // If event type is knock, Remove it from knock_map_.
  int event_type = event_infos_[event_id]->GetEvent()->GetType();
  if (event_type == Event::Type::kKnock) {
    knock_map_[event_name].remove(event_id);
  }

  // Unset infos
  event_infos_[event_id].release();
  handler_infos_[handler_id]->UnsetEventId(event_name);

  // Delete event from epoll
  epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, event_id, nullptr);
  return true;
}

bool EventManager::SendKnock(std::string handler_name,
                             std::string knock_event_name) {
  std::string log_name = handler_name + " - " + knock_event_name;

  int handler_id;
  if ((handler_id = GetHandlerId(handler_name)) == -1) {
    Logger(Logger::kWarn) << "EventManager - No handler - " << log_name;
    return false;
  }

  int event_id;
  if ((event_id = GetEventId(handler_name, knock_event_name)) == -1) {
    Logger(Logger::kWarn) << "EventManager - No event - " << log_name;
    return false;
  }

  // Write to eventfd
  if (eventfd_write(event_id, 1) != 0) {
    Logger(Logger::kWarn) << "EventManager - Send knock failed - " << log_name;
    return false;
  }

  return true;
}

bool EventManager::SendAllKnock(std::string knock_event_name) {
  auto id_list = knock_map_[knock_event_name];
  for (auto iter = id_list.begin(); iter != id_list.end(); iter++) {
    // Write to eventfd
    if (eventfd_write(*iter, 1) != 0) {
      Logger(Logger::kWarn)
          << "EventManager - Send all knock failed - " << knock_event_name;
      return false;
    }
  }

  return true;
}

bool EventManager::PushData(std::string handler_name, std::string queue_name,
                            void* data) {
  std::string log_name = handler_name + " - " + queue_name;

  int handler_id;
  if ((handler_id = GetHandlerId(handler_name)) == -1) {
    Logger(Logger::kWarn) << "No handler - " << log_name;
    return false;
  }

  // Push object
  handler_infos_[handler_id]->GetContext()->PushQueue(queue_name, data);
  return true;
}

bool EventManager::PushData(std::string handler_name, void* data) {
  return PushData(handler_name, HandlerContext::kDefaultQueueName, data);
}
