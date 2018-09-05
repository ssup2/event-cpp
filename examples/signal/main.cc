#include <event/manager.h>
#include <iostream>

#include "handler_signal1.h"
#include "handler_signal2.h"

int main() {
  EventManager* mgr = EventManager::GetManager();

  HandlerSignal1* handler_signal1 = new HandlerSignal1();
  mgr->AddHandler("handler_signal1", handler_signal1);
  HandlerSignal2* handler_signal2 = new HandlerSignal2();
  mgr->AddHandler("handler_signal2", handler_signal2);

  mgr->Run();

  return 0;
}
