//
// Created by shecannotsee on 2023/3/2.
//

#ifndef SHE_NET_SRC_SOCKET_H_
#define SHE_NET_SRC_SOCKET_H_

#include <net_base.h>
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

 private:
  int id_;
  NetTransport net_transport_;

 public:
  void bind(const std::string& ip,const std::string& port) noexcept;
  void listen() noexcept;
  void accept() noexcept;
  void connect() noexcept;

};// class socket

};// namespace sheNet {

#endif //SHE_NET_SRC_SOCKET_H_
