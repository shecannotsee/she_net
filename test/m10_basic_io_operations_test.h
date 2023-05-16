//
// Created by shecannotsee on 23-5-16.
//

#ifndef SHE_NET_TEST_M10_BASIC_IO_OPERATIONS_TEST_H_
#define SHE_NET_TEST_M10_BASIC_IO_OPERATIONS_TEST_H_

#include <basic_io_operations/basic_io_operations.h>

namespace m10_basic_io_operations_test {

void main() {
  /* tcp io */ {
    using tcp_io = sheNet::basic_io_operations::TCP;
    int fd_tcp = 0;
    std::string get_message = tcp_io::recv(fd_tcp);
    tcp_io::send(fd_tcp, "123");
  };

  /* udp io */ {
    using udp_io = sheNet::basic_io_operations::UDP;
    int fd_udp = 0;
    std::string udp_message = udp_io::recvfrom(fd_udp);
    udp_io::sendto("","","");
  }

};

};// namespace m10_basic_io_operations_test

#endif //SHE_NET_TEST_M10_BASIC_IO_OPERATIONS_TEST_H_
