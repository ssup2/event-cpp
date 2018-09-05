#include <event/event.h>
#include <event/manager.h>
#include <logger/logger.h>

#include "handler_knock1.h"

bool HandlerKnock1::Init(HandlerContext* context) {
  EventManager* mgr = EventManager::GetManager();
  std::string handler_name = context->GetHandlerName();

  knock_normal_ = new Knock("knock_normal", Knock::kNormal);
  mgr->AddEvent(handler_name, knock_normal_->GetEvent());

  knock_aggre_ = new Knock("knock_aggre", Knock::kAggre);
  mgr->AddEvent(handler_name, knock_aggre_->GetEvent());

  return true;
}

bool HandlerKnock1::Handler(HandlerContext* context, Event* event) {
  (void)context;
  (void)event;

  if (event->GetId() == knock_normal_->GetEvent()->GetId()) {
    knock_normal_->Clear();
    Logger(Logger::kInfo) << "knock_normal event";
  } else if (event->GetId() == knock_aggre_->GetEvent()->GetId()) {
    knock_aggre_->Clear();
    Logger(Logger::kInfo) << "knock_aggre event";
  } else {
    Logger(Logger::kInfo) << "wrong event";
  }

  return true;
}
