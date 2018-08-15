#ifndef HANDLER_H_
#define HANDLER_H_

#include <list>
#include <map>
#include <memory>

#include "event.h"

class HandlerObj {
 public:
  HandlerObj() {}
  virtual ~HandlerObj() = 0;
};

class HandlerContext {
 public:
  static std::string kDefaultQueueName;

 private:
  int handler_id_;
  std::string handler_name_;

  std::map<std::string, std::list<std::unique_ptr<HandlerObj>>> obj_queues_;

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

  void PushQueue(std::string queue_name, std::unique_ptr<HandlerObj> obj);
  void PushQueue(std::unique_ptr<HandlerObj> obj);

  std::unique_ptr<HandlerObj> PopQueue(std::string queue_name);
  std::unique_ptr<HandlerObj> PopQueue();
};

class EventHandler {
 public:
  enum Return { kSucc = 0, kFail = -1 };

 public:
  virtual ~EventHandler() = 0;

  virtual int Init(HandlerContext* context) = 0;
  virtual int Handler(HandlerContext* context, Event* event) = 0;
};

#endif  // HANDLER_H_
