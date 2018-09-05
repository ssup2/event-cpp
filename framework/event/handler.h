#ifndef HANDLER_H_
#define HANDLER_H_

#include <list>
#include <map>
#include <memory>

#include "event.h"

class HandlerContext {
 public:
  static std::string kDefaultQueueName;

 private:
  int handler_id_;
  std::string handler_name_;

  std::map<std::string, std::list<void*>> data_queues_;

 public:
  HandlerContext(int handler_id, std::string handler_name);
  ~HandlerContext();

  int GetHandlerId();
  std::string GetHandlerName();

  bool IsQueueEmpty(std::string queue_name);
  bool IsQueueEmpty();

  int GetQueueSize(std::string queue_name);
  int GetQueueSize();

  void ClearQueue(std::string queue_name);
  void ClearQueue();

  void PushQueue(std::string queue_name, void* data);
  void PushQueue(void* data);

  void* PopQueue(std::string queue_name);
  void* PopQueue();
};

class EventHandler {
 public:
  virtual ~EventHandler() {}

  virtual bool Init(HandlerContext* context) = 0;
  virtual bool Handler(HandlerContext* context, Event* event) = 0;
};

#endif  // HANDLER_H_
