#include <event/manager.h>
#include <iostream>

#include "handler_knock1.h"
#include "handler_timer1.h"

int main() {
  EventManager* mgr = EventManager::GetManager();

  HandlerTimer1* handler_timer1 = new HandlerTimer1();
  mgr->AddHandler("handler_timer1", handler_timer1);
  HandlerKnock1* handler_knock1 = new HandlerKnock1();
  mgr->AddHandler("handler_knock1", handler_knock1);

  mgr->Run();

  return 0;
}
