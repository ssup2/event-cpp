#include <event/event.h>
#include <event/manager.h>
#include <logger/logger.h>

#include "handler_timer1.h"

bool HandlerTimer1::Init(HandlerContext* context) {
  EventManager* mgr = EventManager::GetManager();
  std::string handler_name = context->GetHandlerName();

  timer_ = new Timer("timer1");
  timer_->SetTimer(1500, 1500);
  mgr->AddEvent(handler_name, timer_->GetEvent());

  return true;
}

bool HandlerTimer1::Handler(HandlerContext* context, Event* event) {
  EventManager* mgr = EventManager::GetManager();
  (void)context;
  (void)event;

  if (event->GetId() == timer_->GetEvent()->GetId()) {
    timer_->Clear();
    Logger(Logger::kInfo) << "timer1 event";

    mgr->SendKnock("handler_knock1", "knock_normal");
    mgr->PushData("handler_knock1", (void*)1);
    mgr->SendKnock("handler_knock1", "knock_normal");
    mgr->PushData("handler_knock1", (void*)2);

    mgr->SendKnock("handler_knock1", "knock_aggre");
    mgr->PushData("handler_knock1", "knock", (void*)3);
    mgr->SendKnock("handler_knock1", "knock_aggre");
    mgr->PushData("handler_knock1", "knock", (void*)4);
  } else {
    Logger(Logger::kInfo) << "wrong event";
  }

  return true;
}
