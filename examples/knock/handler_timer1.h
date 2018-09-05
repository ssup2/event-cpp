#ifndef HANDLER_TIMER1_H_
#define HANDLER_TIMER1_H_

#include <event/handler.h>
#include <helper/timer.h>

class HandlerTimer1 : public EventHandler {
 private:
  Timer* timer_;

 public:
  bool Init(HandlerContext* context);
  bool Handler(HandlerContext* context, Event* event);
};

#endif  // HANDLER_TIMER1_H_
