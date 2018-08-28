#ifndef TCP_CONN_H_
#define TCP_CONN_H_

#include <memory>
#include <string>

#include <arpa/inet.h>
#include <event/event.h>

class TcpConn {
 public:
  TcpConn(std::string event_name, std::string ip, int port);
  ~TcpConn();

  Event* GetEvent();

  bool IsConnected();
  void Close();
  bool Connect();
  bool Reconnect();

  int RecvSync(void* packet, size_t lenght, int flags);
  int SendSync(void* packet, size_t lenght, int flags);

 private:
  std::unique_ptr<Event> event_;
  std::string event_name_;

  int socket_fd_;
  std::string ip_;
  int port_;

 private:
  int RecvAll(int fd, void* buf, size_t buf_size, int flags);
  int SendAll(int fd, void* buf, size_t buf_size, int flags);
};

#endif  // TCP_CONN_H_
