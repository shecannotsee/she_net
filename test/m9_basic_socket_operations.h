//
// Created by shecannotsee on 23-5-15.
//

#ifndef SHE_NET_TEST_M9_BASIC_SOCKET_OPERATIONS_H_
#define SHE_NET_TEST_M9_BASIC_SOCKET_OPERATIONS_H_

#include <basic_socket_operations/basic_socket_operations.h>
#include <four_tuple.h>

namespace m9_basic_socket_operations {

void main() {
  using st/* socket tools*/ = sheNet::basic_socket_operations;
  auto type = sheNet::TRANSPORT_ADDRESS_TYPE::TCP_IPV4;
  sheNet::four_tuple address;/* init */ {
    address.source_ip = "0.0.0.0";
    address.source_port = 9981;
  }
  int fd = st::socket(type);/* print */{
    std::cout << "socket create fd:" << fd <<"\n";
  };
  st::bind(fd,address.source_ip,std::to_string(address.source_port),type);/* print */ {
    std::cout << "bind "+ address.source_ip+":" + std::to_string(address.source_port) +" success.\n";
  };




};

};// namespace m9_basic_socket_operations

#endif //SHE_NET_TEST_M9_BASIC_SOCKET_OPERATIONS_H_
