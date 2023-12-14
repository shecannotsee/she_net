//
// Created by shecannotsee on 2023/3/8.
//

#ifndef SHE_NET_SRC_FOUR_TUPLE_H_
#define SHE_NET_SRC_FOUR_TUPLE_H_

#include <string>

namespace she_net {

class four_tuple {
 public:
  inline four_tuple()
      : source_fd(-1), source_ip(), source_port(0), destination_fd(-1), destination_ip(), destination_port(0){};
  ~four_tuple() = default;

 public:
  // local fd
  int source_fd;
  // local ip
  std::string source_ip;
  // Port used locally
  unsigned int source_port;

  // local fd to connect to remote
  int destination_fd;
  // remote ip
  std::string destination_ip;
  // remote port
  unsigned int destination_port;

};  // class four_tuple

};  // namespace she_net

#endif  // SHE_NET_SRC_FOUR_TUPLE_H_
