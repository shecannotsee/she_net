//
// Created by shecannotsee on 2023/3/2.
//

#ifndef SHE_NET_SRC_SOCKET_H_
#define SHE_NET_SRC_SOCKET_H_

namespace sheNet {

class socket {
 public:
  socket();
 private:

 public:
  bind();
  listen();
  accept();
  connect();
  close();

};// class socket

};// namespace sheNet {

#endif //SHE_NET_SRC_SOCKET_H_
