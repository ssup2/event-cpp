#ifndef HANDLER_SIGNAL1_H_
#define HANDLER_SIGNAL1_H_

#include <event/handler.h>
#include <helper/signal.h>

class HandlerSignal1 : public EventHandler {
 private:
  Signal* signal_;

 public:
  bool Init(HandlerContext* context);
  bool Handler(HandlerContext* context, Event* event);
};

#endif  // HANDLER_SIGNAL1_H_
