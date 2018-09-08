#ifndef HANDLER_KNOCK2_H_
#define HANDLER_KNOCK2_H_

#include <event/handler.h>
#include <helper/knock.h>

class HandlerKnock2 : public EventHandler {
 private:
  Knock* knock_normal_;
  Knock* knock_aggre_;
  Knock* knock_all_;

 public:
  bool Init(HandlerContext* context);
  bool Handler(HandlerContext* context, Event* event);
};

#endif  // HANDLER_KNOCK2_H_
