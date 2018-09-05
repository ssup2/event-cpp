#ifndef HANDLER_KNOCK1_H_
#define HANDLER_KNOCK1_H_

#include <event/handler.h>
#include <helper/knock.h>

class HandlerKnock1 : public EventHandler {
 private:
  Knock* knock_normal_;
  Knock* knock_aggre_;

 public:
  bool Init(HandlerContext* context);
  bool Handler(HandlerContext* context, Event* event);
};

#endif  // HANDLER_KNOCK1_H_
