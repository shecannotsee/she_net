//
// Created by shecannotsee on 24-2-1.
//

#ifndef T1_SOCKET_H
#define T1_SOCKET_H

#include <basic_socket_operations/bind.h>
#include <basic_socket_operations/socket.h>

#include <type_traits>

#include "basic_socket_operations/listen.h"

namespace t1_socket {
void run() {
  using namespace she_net;
  // socket
  auto fd = socket_t::create<socket_param::ipv4, socket_param::stream, socket_param::tcp>();
  // bind
  bind_t::exec<local_l, socket_param::ipv4, port, 19918>(fd);
  // listen
  listen_t::exec<socket_param::stream>(fd);

  std::cout << "success" << std::endl;
}

}  // namespace t1_socket

#endif  // T1_SOCKET_H
