//
// Created by shecannotsee on 2023/3/7.
//

#ifndef SHE_NET_TEST_M4_UDP_IO_TEST_H_
#define SHE_NET_TEST_M4_UDP_IO_TEST_H_

#include <iostream>
#include <string>
#include <future>
#include <memory>

#include "high_coupling_model/socket.h"
#include "high_coupling_model/message.h"
#include <transport_address_type.h>
#include <make_unique.h>

#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <arpa/inet.h>

namespace m4_udp_io_test {

int main() {
  std::string ip = "192.168.1.47";
  std::string port = "9981";

  auto client = [&]() {
    try {
      she_net::TRANSPORT_ADDRESS_TYPE udp = she_net::TRANSPORT_ADDRESS_TYPE::UDP_IPV4;
      std::unique_ptr<she_net::socket> client = she_net::CPP11::make_unique<she_net::socket>(udp);/* upd client init */ {
        she_net::four_tuple quadruple;
        quadruple.destination_ip = ip;
        quadruple.destination_port = std::atoi(port.c_str());
        client->udp_set(std::move(quadruple));
      };
      she_net::message message_control(std::move(client));
      std::cout << "udp client set done,start to send message.\n";
      while (1) {
        message_control.send("hello world");
        std::cout << "send done.\n";
        sleep(2);
      }

    } catch (const std::exception& exc) {
      std::cout<<exc.what()<<std::endl;
    }
  };

  auto server = [&]() {
    try {
      she_net::TRANSPORT_ADDRESS_TYPE udp = she_net::TRANSPORT_ADDRESS_TYPE::UDP_IPV4;
      std::unique_ptr<she_net::socket> server = she_net::CPP11::make_unique<she_net::socket>(udp);/* upd server init */ {
        server->bind(port);
      };
      she_net::message message_control(std::move(server));
      std::cout << "udp server set done.\n";
      while (1) {
        std::cout << "wait to data...\n";
        std::string data = message_control.get();
        std::cout << "data is:[" << data << "]\n";
        sleep(1);
      }

    } catch (const std::exception& exc) {
      std::cout<<exc.what()<<std::endl;
    }
  };

  auto client_future = std::async(std::launch::async,client);
  auto server_future = std::async(std::launch::async,server);

  sleep(100);
  return 0;
};

};// namespace m4_udp_io_test

#endif //SHE_NET_TEST_M4_UDP_IO_TEST_H_
