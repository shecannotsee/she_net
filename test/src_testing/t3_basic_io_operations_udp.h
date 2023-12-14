//
// Created by shecannotsee on 23-12-14.
//

#ifndef T3_BASIC_IO_OPERATIONS_UDP_H
#define T3_BASIC_IO_OPERATIONS_UDP_H

#include <string>
#include <iostream>
#include <thread>
#include <basic_socket_operations/basic_socket_operations.h>
#include <basic_io_operations/basic_io_operations.h>
#include <she_net_exception/she_net_exception.h>

namespace src_testing::t3_basic_io_operations_udp {

std::string main() {
  std::cout << RED_COLOR << "basic_io_operations udp test start." << RESET_COLOR << std::endl;

  auto client = std::thread([](){
    /* create socket */
    using BSO = she_net::basic_socket_operations;
    int client_fd = BSO::socket(she_net::TRANSPORT_ADDRESS_TYPE::UDP_IPV4);

    /* io */
    using io = she_net::basic_io_operations::UDP;
    while (true) {
      try {
        sleep(1);
        static int message_num = 0;
        io::sendto(client_fd, "192.168.1.47", "9981", "No." + std::to_string(++message_num) + " message");
        std::cout << YELLOW_COLOR << "[" << "No." + std::to_string(message_num) + " message] has been sent.\n" << RESET_COLOR;
      }
      catch (const she_net::she_net_exception& exc) {
        std::cout << YELLOW_COLOR << exc.what() << RESET_COLOR;
        break;
      }
    }

    BSO::shutdown(client_fd);
    std::cout << YELLOW_COLOR << "udp client closed.\n" << RESET_COLOR;
  });

  auto server = std::thread([](){
    /* create socket */
    using BSO = she_net::basic_socket_operations;
    auto UDP_IPV4 = she_net::TRANSPORT_ADDRESS_TYPE::UDP_IPV4;
    int server_fd = BSO::socket(UDP_IPV4);/* set */ {
      BSO::port_reuse(server_fd);
      // BSO::set_socket_noblock(server_fd);// Using no-blocking can lead to an increase in CPU usage (single core)
      BSO::bind(server_fd,"0.0.0.0","9981",UDP_IPV4);
    };

    /* io */
    using io = she_net::basic_io_operations::UDP;
    while (true) {
      try {
        std::string message_get = io::recvfrom(server_fd);
        if (message_get.size() > 0) {
          std::cout << GREEN_COLOR << "[" << message_get << "]\n" << RESET_COLOR;
        } else {
          // std::cout << GREEN_COLOR << message_get.size();
        }
      }
      catch (const she_net::she_net_exception& exc) {
        std::cout << GREEN_COLOR << exc.what() << "\n" << RESET_COLOR;
        break;
      }
    }
    BSO::shutdown(server_fd);
    std::cout << GREEN_COLOR << "udp server closed.\n" << RESET_COLOR;
  });

  client.join();
  server.join();

  return {};
}

}

#endif //T3_BASIC_IO_OPERATIONS_UDP_H
