#ifndef SOCKET_H_
#define SOCKET_H_

#include <string>

#include <arpa/inet.h>
#include <event/event.h>

class Socket {
 private:
  int socket_fd_;

  std::string ip_;
  int port_;
  sockaddr_in server_addr_;

 public:
  Socket(std::string ip, int port);
  ~Socket();

  Event* GetEvent(std::string event_name);

  int RecvSync(void* packet, size_t lenght, int flags);
  int SendSync(void* packet, size_t lenght, int flags);

  bool IsConnected();

  int Close();
  int Connect();
  int Reconnect();

 private:
  int RecvAll(int fd, void* buf, size_t buf_size, int flags);
  int SendAll(int fd, void* buf, size_t buf_size, int flags);
};

#endif  // SOCKET_H_
