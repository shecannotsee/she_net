//
// Created by shecannotsee on 2023/3/2.
//

#ifndef SHE_NET_SRC_SOCKET_H_
#define SHE_NET_SRC_SOCKET_H_

#include <net_transport.h>
#include <client_info.h>
#include <string>

namespace sheNet {

class socket {
 public:
  socket();
  explicit socket(NetTransport) noexcept;
  ~socket();
  // disable copy
  socket(const socket& x) = delete;
  socket& operator=(const socket& x) = delete;
  // move
  socket(socket&&) = default;
  socket& operator=(socket&& x) = default;
 private:
  int id_;
  NetTransport net_transport_;

 public:
  void bind(const std::string& ip, std::string& port) noexcept;
  void listen(int backlog = 5) noexcept;
  ClientInfo accept() noexcept;

  void connect(const std::string& port,const std::string& ip = "0.0.0.0") noexcept;

 public:
  int get_id() const;
  NetTransport get_net_transport() const;

};// class socket

};// namespace sheNet {

#endif //SHE_NET_SRC_SOCKET_H_
