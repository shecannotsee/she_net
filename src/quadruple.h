//
// Created by shecannotsee on 2023/3/8.
//

#ifndef SHE_NET_SRC_QUADRUPLE_H_
#define SHE_NET_SRC_QUADRUPLE_H_

#include <string>

namespace sheNet {

class quadruple {
 public:
  inline quadruple()
      : source_fd(-1),source_ip(),source_port(0),
        destination_fd(-1),destination_ip(),destination_port(0) {};
  ~quadruple()=default;
 public:
  int source_fd;
  std::string source_ip;
  unsigned int source_port;

  int destination_fd;
  std::string destination_ip;
  unsigned int destination_port;

};// class quadruple

};// namespace sheNet {

#endif //SHE_NET_SRC_QUADRUPLE_H_
