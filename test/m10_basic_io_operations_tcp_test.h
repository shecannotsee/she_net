//
// Created by shecannotsee on 23-5-16.
//

#ifndef SHE_NET_TEST_M10_BASIC_IO_OPERATIONS_TEST_H_
#define SHE_NET_TEST_M10_BASIC_IO_OPERATIONS_TEST_H_

#include <basic_io_operations/basic_io_operations.h>

namespace m10_basic_io_operations_tcp_test {

void main() {
  /* tcp io */ {
    using tcp_io = sheNet::basic_io_operations::TCP;
    int fd_tcp = 0;
    std::string get_message = tcp_io::recv(fd_tcp);
    tcp_io::send(fd_tcp, "123");
  };

};

};// namespace m10_basic_io_operations_test

#endif //SHE_NET_TEST_M10_BASIC_IO_OPERATIONS_TEST_H_
