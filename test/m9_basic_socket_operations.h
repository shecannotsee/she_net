//
// Created by shecannotsee on 23-5-15.
//

#ifndef SHE_NET_TEST_M9_BASIC_SOCKET_OPERATIONS_H_
#define SHE_NET_TEST_M9_BASIC_SOCKET_OPERATIONS_H_

#include <basic_socket_operations/basic_socket_operations.h>

namespace m9_basic_socket_operations {

void main() {
  using st/* socket tools*/ = sheNet::basic_socket_operations;
  int fd = st::socket(sheNet::TRANSPORT_ADDRESS_TYPE::TCP_IPV4);


};

};// namespace m9_basic_socket_operations

#endif //SHE_NET_TEST_M9_BASIC_SOCKET_OPERATIONS_H_
