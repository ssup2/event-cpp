#ifndef HANDLER_TIMER2_H_
#define HANDLER_TIMER2_H_

#include <event/handler.h>
#include <helper/timer.h>

class HandlerTimer2 : public EventHandler {
 private:
  Timer* timer_;

 public:
  bool Init(HandlerContext* context);
  bool Handler(HandlerContext* context, Event* event);
};

#endif  // HANDLER_TIMER2_H_
