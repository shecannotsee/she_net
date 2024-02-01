//
// Created by shecannotsee on 24-2-1.
//

#ifndef T1_SOCKET_H
#define T1_SOCKET_H

#include <basic_socket_operations/socket.h>
#include <type_traits>

namespace t1_socket {
void run() {
  using namespace she_net;
  auto r = socket_t::create<ipv4, stream, udp>();
  std::cout << static_cast<int>(r) << std::endl;
}

}

#endif //T1_SOCKET_H
