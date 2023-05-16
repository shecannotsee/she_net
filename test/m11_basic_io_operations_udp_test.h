//
// Created by shecannotsee on 23-5-16.
//

#ifndef SHE_NET_M11_BASIC_IO_OPERATIONS_UDP_TEST_H
#define SHE_NET_M11_BASIC_IO_OPERATIONS_UDP_TEST_H

#include <basic_io_operations/basic_io_operations.h>

namespace m11_basic_io_operations_udp_test {

void main() {
  /* udp io */ {
    using udp_io = sheNet::basic_io_operations::UDP;
    int fd_udp = 0;
    std::string udp_message = udp_io::recvfrom(fd_udp);
    udp_io::sendto(fd_udp,"","","");
  }

};

};// namespace m11_basic_io_operations_udp_test

#endif //SHE_NET_M11_BASIC_IO_OPERATIONS_UDP_TEST_H
