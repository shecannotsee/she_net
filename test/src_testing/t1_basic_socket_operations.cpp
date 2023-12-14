//
// Created by shecannotsee on 23-12-14.
//

#include "t1_basic_socket_operations.h"

#include <iostream>
#include <thread>

#include <basic_socket_operations/basic_socket_operations.h>

namespace {
const std::string ip = "0.0.0.0";
constexpr int port = 9981;
using st/* socket tools*/ = she_net::basic_socket_operations;
bool server_start = false;// use to flag server start
auto tcp_ipv4 = she_net::TRANSPORT_ADDRESS_TYPE::TCP_IPV4;
}

std::string src_testing::t1_basic_socket_operations::main() {

  std::thread server([&]() {
    int fd = st::socket(tcp_ipv4);/* print */{
      std::cout << "server:socket create fd:" << fd << "\n";
    };
    st::port_reuse(fd,1);
    st::bind(fd, ip, std::to_string(port), tcp_ipv4);/* print */ {
      std::cout << "server:bind " + ip + ":" + std::to_string(port) + " success.\n";
    };
    st::listen(fd);/* print */ {
      std::cout << "server:listen success.\n";
    };
    /* print */ {
      std::cout << "server:wait to accept...\n";
    }

    while(true) {
      server_start = true;
      int client_fd = st::accept(fd);/* print */ {
        std::cout << "server:accept to client fd is " << client_fd << "\n";
      }
      if(client_fd!= -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        break;
      }
    }
    st::shutdown(fd);
    std::cout << "server close.\n";
  });

  std::thread client([&](){
    while (server_start == false) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    int fd = st::socket(tcp_ipv4);/* print */{
      std::cout << "client:socket create fd:" << fd << "\n";
    };

    int used_port = -1;
    while (used_port == -1) {
      used_port = st::connect(fd, "0.0.0.0", "9981");/* print */ {
          std::cout << "client:connect success,local port is[" + std::to_string(used_port) + "]\n";
        }
    }

    st::shutdown(fd);
    std::cout << "client close.\n";
  });


  server.join();
  client.join();

  return "t1_basic_socket_operations pass test";
}
