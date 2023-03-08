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
  std::string ip = "192.168.1.47";
  std::string port = "9981";

  auto client = [&]() {
    try {
      sheNet::NetTransport udp = sheNet::NetTransport::UDP_IPV4;
      std::unique_ptr<sheNet::socket> client = sheNet::CPP11::make_unique<sheNet::socket>(udp);/* upd client init */ {
        sheNet::quadruple quadruple;
        quadruple.destination_ip = ip;
        quadruple.destination_port = std::atoi(port.c_str());
        client->udp_set(std::move(quadruple));
      };
      sheNet::message message_control(std::move(client));
      std::cout << "udp client set done.\n";
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
      sheNet::NetTransport udp = sheNet::NetTransport::UDP_IPV4;
      std::unique_ptr<sheNet::socket> server = sheNet::CPP11::make_unique<sheNet::socket>(udp);/* upd server init */ {
        server->bind(port);
      };
      sheNet::message message_control(std::move(server));
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

  auto server_future = std::async(std::launch::async,server);
  auto client_future = std::async(std::launch::async,client);

  sleep(100);
  return 0;
};

};// namespace m4_udp_io_test

#endif //SHE_NET_TEST_M4_UDP_IO_TEST_H_
