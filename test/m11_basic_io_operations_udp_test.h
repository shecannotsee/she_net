//
// Created by shecannotsee on 23-5-16.
//

#ifndef SHE_NET_M11_BASIC_IO_OPERATIONS_UDP_TEST_H
#define SHE_NET_M11_BASIC_IO_OPERATIONS_UDP_TEST_H

#include <thread>
#include <basic_socket_operations/basic_socket_operations.h>
#include <basic_io_operations/basic_io_operations.h>

namespace m11_basic_io_operations_udp_test {

const std::string RESET_COLOR   = "\033[0m";
const std::string RED_COLOR     = "\033[31m";
const std::string GREEN_COLOR   = "\033[32m";
const std::string YELLOW_COLOR  = "\033[33m";

void main() {
  std::cout << RED_COLOR << "basic_io_operations udp test start." << RESET_COLOR << std::endl;

  auto client = std::thread([](){
    /* create socket */
    using BSO = sheNet::basic_socket_operations;
    int client_fd = BSO::socket(sheNet::TRANSPORT_ADDRESS_TYPE::UDP_IPV4);

    /* io */
    using io = sheNet::basic_io_operations::UDP;
    while (true) {
      try {
        sleep(1);
        static int message_num = 0;
        io::sendto(client_fd, "192.168.1.47", "9981", "No." + std::to_string(++message_num) + " message");
        std::cout << YELLOW_COLOR << "[" << "No." + std::to_string(message_num) + " message] has been sent.\n" << RESET_COLOR;
        sleep(999);
      }
      catch (const sheNet::sheNetException& exc) {
        std::cout << YELLOW_COLOR << exc.what() << RESET_COLOR;
        break;
      }
    }

    BSO::shutdown(client_fd);
    std::cout << YELLOW_COLOR << "udp client closed.\n" << RESET_COLOR;
  });

  auto server = std::thread([](){
    /* create socket */
    using BSO = sheNet::basic_socket_operations;
    auto UDP_IPV4 = sheNet::TRANSPORT_ADDRESS_TYPE::UDP_IPV4;
    int server_fd = BSO::socket(UDP_IPV4);/* set */ {
      BSO::port_reuse(server_fd);
      // BSO::set_socket_noblock(server_fd);// Using no-blocking can lead to an increase in CPU usage (single core)
      BSO::bind(server_fd,"0.0.0.0","9981",UDP_IPV4);
    };

    /* io */
    using io = sheNet::basic_io_operations::UDP;
    while (true) {
      try {
        std::string message_get = io::recvfrom(server_fd);
        if (message_get.size() > 0) {
          std::cout << GREEN_COLOR << "[" << message_get << "]\n" << RESET_COLOR;
        } else {
          // std::cout << GREEN_COLOR << message_get.size();
        }
      }
      catch (const sheNet::sheNetException& exc) {
        std::cout << GREEN_COLOR << exc.what() << "\n" << RESET_COLOR;
        break;
      }
    }
    BSO::shutdown(server_fd);
    std::cout << GREEN_COLOR << "udp server closed.\n" << RESET_COLOR;
  });

  client.join();
  server.join();

};

};// namespace m11_basic_io_operations_udp_test

#endif //SHE_NET_M11_BASIC_IO_OPERATIONS_UDP_TEST_H
