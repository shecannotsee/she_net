//
// Created by shecannotsee on 23-5-16.
//

#ifndef SHE_NET_TEST_M10_BASIC_IO_OPERATIONS_TEST_H_
#define SHE_NET_TEST_M10_BASIC_IO_OPERATIONS_TEST_H_

#include <iostream>
#include <thread>
#include <basic_socket_operations/basic_socket_operations.h>
#include <basic_io_operations/basic_io_operations.h>

namespace m10_basic_io_operations_tcp_test {

const std::string RESET_COLOR   = "\033[0m";
const std::string RED_COLOR     = "\033[31m";
const std::string GREEN_COLOR   = "\033[32m";
const std::string YELLOW_COLOR  = "\033[33m";

void main() {
  std::cout << RED_COLOR << "test start." << RESET_COLOR << std::endl;
  sleep(1);

  auto client = std::thread([](){
    /* connect */
    using BSO = sheNet::basic_socket_operations;
    int client_fd = BSO::socket();/* set */ {
      BSO::set_socket_noblock(client_fd);
      BSO::port_reuse(client_fd);
    };
    while (true) {
      try {
        BSO::connect(client_fd, "192.168.1.47", "9981");
        break;
      }
      catch (const sheNet::sheNetException& exc) {
        if (exc.get_error_code() == 21) {
          sleep(1);
          continue;
        } else {
          std::cout << YELLOW_COLOR << "connect failed." << RESET_COLOR << std::endl;
          sleep(1);
          continue;
        }
      }
    };

    /* io */
    using io = sheNet::basic_io_operations::TCP;

    while (1) {
      io::send(client_fd, "");
      sleep(1);
    }


    BSO::shutdown(client_fd);
  });

  auto server = std::thread([](){
    using socket = sheNet::basic_socket_operations;
    using io = sheNet::basic_io_operations::TCP;

  });

  client.join();
  server.join();

};

};// namespace m10_basic_io_operations_test

#endif //SHE_NET_TEST_M10_BASIC_IO_OPERATIONS_TEST_H_
