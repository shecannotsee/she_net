//
// Created by shecannotsee on 2023/3/7.
//

#ifndef SHE_NET_TEST_M3_TCP_IO_TEST_H_
#define SHE_NET_TEST_M3_TCP_IO_TEST_H_

#include <iostream>
#include <string>
#include <future>
#include <memory>

#include "high_coupling_model/socket.h"
#include "high_coupling_model/message.h"
#include <transport_address_type.h>
#include <make_unique.h>

#include <unistd.h>

namespace m3_tcp_io_test {

int main() {
  std::string ip = "192.168.1.47";
  std::string port = "9981";

  auto server = [&]() {
    try {
      she_net::TRANSPORT_ADDRESS_TYPE tcp = she_net::TRANSPORT_ADDRESS_TYPE::TCP_IPV4;
      std::unique_ptr<she_net::socket> server = she_net::CPP11::make_unique<she_net::socket>(tcp);/* tcp init */{
        server->bind(port);
        server->listen();
        server->accept();
      };
      she_net::message message_control(std::move(server));
      std::cout << "server set done.\n";
      int on = 1;
      while (on--) {
        std::string data = message_control.get();
        std::cout << "data is:[" << data << "]\n";
        sleep(3);
      }
    } catch (const std::exception& exc) {
      std::cout<<exc.what()<<std::endl;
    }
    std::cout << "server close.\n";
  };

  auto client = [&]() {
    try {
      she_net::TRANSPORT_ADDRESS_TYPE tcp = she_net::TRANSPORT_ADDRESS_TYPE::TCP_IPV4;
      std::unique_ptr<she_net::socket> client = she_net::CPP11::make_unique<she_net::socket>(tcp);/* tcp init */ {
        client->connect(ip, port);
      };
      she_net::message message_control(std::move(client));
      std::cout << "client set done.\n";
      while (1) {
        message_control.send("hello world");
        std::cout<< "send done.\n";
        sleep(2);
      }
    } catch (const std::exception& exc) {
      std::cout<<exc.what()<<std::endl;
    }
  };

  auto server_future = std::async(std::launch::async,server);
  auto client_future = std::async(std::launch::async,client);
  sleep(100);
  return 0;
};

};// namespace m3_tcp_io_test

#endif //SHE_NET_TEST_M3_TCP_IO_TEST_H_
