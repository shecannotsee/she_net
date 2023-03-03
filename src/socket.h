//
// Created by shecannotsee on 2023/3/2.
//

#ifndef SHE_NET_SRC_SOCKET_H_
#define SHE_NET_SRC_SOCKET_H_

#include <net_base.h>

namespace sheNet {

class socket {
 public:
  socket();
  explicit socket(NetTransport) noexcept;
 private:
  int id_;

 public:
//  bind();
//  listen();
//  accept();
//  connect();
//  close();

};// class socket

};// namespace sheNet {

#endif //SHE_NET_SRC_SOCKET_H_
