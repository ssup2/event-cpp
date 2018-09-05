#include <event/event.h>
#include <event/manager.h>
#include <logger/logger.h>

#include "handler_timer2.h"

bool HandlerTimer2::Init(HandlerContext* context) {
  EventManager* mgr = EventManager::GetManager();
  std::string handler_name = context->GetHandlerName();

  timer_ = new Timer("timer2");
  timer_->SetTimer(2500, 0);
  mgr->AddEvent(handler_name, timer_->GetEvent());

  return true;
}

bool HandlerTimer2::Handler(HandlerContext* context, Event* event) {
  (void)context;
  (void)event;

  if (event->GetId() == timer_->GetEvent()->GetId()) {
    timer_->Clear();
    timer_->SetTimer(2500, 0);
    Logger(Logger::kInfo) << "timer2 event";
  } else {
    Logger(Logger::kInfo) << "wrong event";
  }

  return true;
}
