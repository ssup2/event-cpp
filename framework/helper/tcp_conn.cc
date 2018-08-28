#include <unistd.h>
#include <cstdio>
#include <cstring>

#include <sys/socket.h>

#include "tcp_conn.h"

TcpConn::TcpConn(std::string event_name, std::string ip, int port) {
  socket_fd_ = -1;

  event_name_ = event_name;
  event_.release();

  ip_ = ip;
  port_ = port;
}

TcpConn::~TcpConn() {
  if (socket_fd_ != -1) close(socket_fd_);
}

Event* TcpConn::GetEvent() { return event_.get(); }

bool TcpConn::IsConnected() { return (socket_fd_ != -1); }

void TcpConn::Close() {
  event_.release();

  close(socket_fd_);
  socket_fd_ = -1;
}

bool TcpConn::Connect() {
  if (IsConnected()) return -1;

  socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd_ == -1) {
    socket_fd_ = -1;
    return false;
  }

  sockaddr_in server_addr;
  memset(&server_addr, sizeof(sockaddr_in), 0);
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(ip_.c_str());
  server_addr.sin_port = htons(port_);

  if (connect(socket_fd_, (sockaddr*)&server_addr, sizeof(sockaddr_in)) < 0) {
    close(socket_fd_);
    socket_fd_ = -1;
    return false;
  }

  event_.reset(new Event(socket_fd_, event_name_));
  return true;
}

bool TcpConn::Reconnect() {
  Close();
  return Connect();
}

int TcpConn::RecvSync(void* packet, size_t lenght, int flags) {
  return RecvAll(socket_fd_, packet, lenght, flags);
}

int TcpConn::SendSync(void* packet, size_t length, int flags) {
  return SendAll(socket_fd_, packet, length, flags);
}

int TcpConn::RecvAll(int fd, void* buf, size_t buf_size, int flags) {
  int recv_size;
  unsigned int recv_total = 0;

  while (recv_total < buf_size) {
    recv_size = recv(fd, (char*)buf + recv_total, buf_size - recv_total, flags);

    if (recv_size == 0)
      break;
    else if (recv_size == -1)
      return -1;

    recv_total += recv_size;
  }

  return recv_total;
}

int TcpConn::SendAll(int fd, void* buf, size_t buf_size, int flags) {
  int send_size;
  unsigned int send_total = 0;

  while (send_total < buf_size) {
    send_size = send(fd, (char*)buf + send_total, buf_size - send_total, flags);

    if (send_size == 0)
      break;
    else if (send_size == -1)
      return -1;

    send_total += send_size;
  }

  return send_total;
}
