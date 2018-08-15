#include "handler.h"

std::string HandlerContext::kDefaultQueueName = "Default";

HandlerContext::HandlerContext(int handler_id, std::string handler_name) {
  handler_id_ = handler_id;
  handler_name_ = handler_name;
}

HandlerContext::~HandlerContext() {}

int HandlerContext::GetHandlerId() { return handler_id_; }

std::string HandlerContext::GetHandlerName() { return handler_name_; }

bool HandlerContext::IsQueueEmpty(std::string queue_name) {
  return obj_queues_[queue_name].empty();
}

bool HandlerContext::IsQueueEmpty() { return IsQueueEmpty(kDefaultQueueName); }

int HandlerContext::GetQueueSize(std::string queue_name) {
  return obj_queues_[queue_name].size();
}

int HandlerContext::GetQueueSize() { return GetQueueSize(kDefaultQueueName); }

void HandlerContext::ClearQueue(std::string queue_name) {
  obj_queues_[queue_name].clear();
}

void HandlerContext::ClearQueue() { ClearQueue(kDefaultQueueName); }

void HandlerContext::PushQueue(std::string queue_name,
                               std::unique_ptr<HandlerObj> obj) {
  obj_queues_[queue_name].push_back(std::move(obj));
}

void HandlerContext::PushQueue(std::unique_ptr<HandlerObj> obj) {
  PushQueue(kDefaultQueueName, std::move(obj));
}

std::unique_ptr<HandlerObj> HandlerContext::PopQueue(std::string queue_name) {
  std::unique_ptr<HandlerObj> ptr = std::move(obj_queues_[queue_name].front());
  obj_queues_[queue_name].pop_front();
  return ptr;
}

std::unique_ptr<HandlerObj> HandlerContext::PopQueue() {
  return PopQueue(kDefaultQueueName);
}
