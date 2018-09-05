#include <event/event.h>
#include <event/manager.h>
#include <logger/logger.h>

#include <csignal>
#include <cstring>

#include "handler_signal1.h"

bool HandlerSignal1::Init(HandlerContext* context) {
  EventManager* mgr = EventManager::GetManager();
  std::string handler_name = context->GetHandlerName();

  signal_ = new Signal("signal1");
  signal_->AddSignal(SIGUSR1);
  signal_->AddSignal(SIGUSR2);
  signal_->SetSignal();
  mgr->AddEvent(handler_name, signal_->GetEvent());

  return true;
}

bool HandlerSignal1::Handler(HandlerContext* context, Event* event) {
  (void)context;
  (void)event;

  if (event->GetId() == signal_->GetEvent()->GetId()) {
    int signal = signal_->GetSignal();
    Logger(Logger::kInfo) << "signal1 event - " << strsignal(signal);
  } else {
    Logger(Logger::kInfo) << "wrong event";
  }

  return true;
}
