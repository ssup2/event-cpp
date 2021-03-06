#include <event/manager.h>
#include <iostream>

#include "handler_timer1.h"
#include "handler_timer2.h"

int main() {
  EventManager* mgr = EventManager::GetManager();

  HandlerTimer1* handler_timer1 = new HandlerTimer1();
  mgr->AddHandler("handler_timer1", handler_timer1);
  HandlerTimer2* handler_timer2 = new HandlerTimer2();
  mgr->AddHandler("handler_timer2", handler_timer2);

  mgr->Run();

  return 0;
}
