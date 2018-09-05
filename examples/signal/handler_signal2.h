#ifndef HANDLER_SIGNAL2_H_
#define HANDLER_SIGNAL2_H_

#include <event/handler.h>
#include <helper/signal.h>

class HandlerSignal2 : public EventHandler {
 private:
  Signal* signal_;

 public:
  bool Init(HandlerContext* context);
  bool Handler(HandlerContext* context, Event* event);
};

#endif  // HANDLER_SIGNAL2_H_
