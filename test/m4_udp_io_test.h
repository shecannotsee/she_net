//
// Created by shecannotsee on 2023/3/7.
//

#ifndef SHE_NET_TEST_M4_UDP_IO_TEST_H_
#define SHE_NET_TEST_M4_UDP_IO_TEST_H_

#include <iostream>
#include <string>
#include <future>
#include <memory>

#include <socket.h>
#include <message.h>
#include <net_transport.h>
#include <make_unique.h>

#include <unistd.h>

namespace m4_udp_io_test {

int main() {
  auto server = []() {
    try {

    } catch (const std::exception& exc) {
      std::cout<<exc.what()<<std::endl;
    }
  };

  auto client = []() {
    try {

    } catch (const std::exception& exc) {
      std::cout<<exc.what()<<std::endl;
    }
  };

  auto server_future = std::async(std::launch::async,server);
  auto client_future = std::async(std::launch::async,client);

  return 0;
};

};// namespace m4_udp_io_test

#endif //SHE_NET_TEST_M4_UDP_IO_TEST_H_
