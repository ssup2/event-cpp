#include <event/manager.h>
#include <iostream>

#include "handler_timer1.h"

int main() {
  EventManager* mgr = EventManager::GetManager();

  HandlerTimer1* handler_timer1 = new HandlerTimer1();
  mgr->AddHandler("hander_timer1", handler_timer1);

  mgr->Run();

  return 0;
}
