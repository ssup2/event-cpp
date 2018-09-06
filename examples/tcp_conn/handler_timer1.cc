#include <event/event.h>
#include <event/manager.h>
#include <logger/logger.h>

#include "handler_timer1.h"

bool HandlerTimer1::Init(HandlerContext* context) {
  EventManager* mgr = EventManager::GetManager();
  std::string handler_name = context->GetHandlerName();

  timer_ = new Timer("timer1");
  timer_->SetTimer(1500, 1500);
  mgr->AddEvent(handler_name, timer_->GetEvent());

  tcp_conn_ = new TcpConn("tcp_conn1", "127.0.0.1", 5000);
  return true;
}

bool HandlerTimer1::Handler(HandlerContext* context, Event* event) {
  (void)context;
  (void)event;

  if (event->GetId() == timer_->GetEvent()->GetId()) {
    timer_->Clear();
    Logger(Logger::kInfo) << "timer1 event";

    if (!tcp_conn_->IsConnected()) {
      if (!tcp_conn_->Connect()) {
        Logger(Logger::kInfo) << "tcp_conn1 Connect() error";
        return false;
      }
    }

    int result;
    std::string data = "data";
    char recv[5] = {};

    result = tcp_conn_->SendSync((void*)data.c_str(), data.length(), 0);
    if (result != (int)data.length()) {
      Logger(Logger::kInfo) << "tcp_conn1 SendSync() error";
      tcp_conn_->Close();
      return false;
    }
    Logger(Logger::kInfo) << "tcp_conn1 send - " << data;

    result = tcp_conn_->RecvSync((void*)&recv, data.length(), 0);
    if (result != (int)data.length()) {
      Logger(Logger::kInfo) << "tcp_conn1 RecvSync() error";
      tcp_conn_->Close();
      return false;
    }
    Logger(Logger::kInfo) << "tcp_conn1 recv - " << recv;

  } else {
    Logger(Logger::kInfo) << "wrong event";
  }

  return true;
}
