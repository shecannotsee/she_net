//
// Created by shecannotsee on 2023/3/7.
//

#ifndef SHE_NET_TEST_M3_TCP_IO_TEST_H_
#define SHE_NET_TEST_M3_TCP_IO_TEST_H_

#include <iostream>
#include <string>
#include <future>
#include <memory>

#include <socket.h>
#include <message.h>
#include <net_transport.h>
#include <make_unique.h>

#include <unistd.h>

namespace m3_tcp_io_test {

int main() {
  auto server = []() {
    try {
      sheNet::NetTransport tcp = sheNet::NetTransport::TCP_IPV4;
      sheNet::socket server_fd(tcp);
      server_fd.bind("9981");
      server_fd.listen();
      sheNet::ClientInfo client_info = server_fd.accept();
      std::unique_ptr<sheNet::socket> client_fd = sheNet::CPP11::make_unique<sheNet::socket>(tcp);
      client_fd->set_id(*client_info.fd_);
      sheNet::message message_control(std::move(client_fd));
      std::cout << "server set done.\n";
      while (1) {
        std::string data = message_control.get();
        std::cout << "data is:[" << data << "]\n";
        sleep(1);
      }
    } catch (const std::exception& exc) {
      std::cout<<exc.what()<<std::endl;
    }
  };

  auto client = []() {
    try {
      sheNet::NetTransport tcp = sheNet::NetTransport::TCP_IPV4;
      std::unique_ptr<sheNet::socket> client_fd = sheNet::CPP11::make_unique<sheNet::socket>(tcp);
      client_fd->connect("192.168.1.65", "9981");
      sheNet::message message_control(std::move(client_fd));
      std::cout << "client set done.\n";
      while (1) {
        message_control.send("hello world");
        sleep(2);
      }
    } catch (const std::exception& exc) {
      std::cout<<exc.what()<<std::endl;
    }
  };

  auto result1 = std::async(std::launch::async,server);
  auto result2 = std::async(std::launch::async,client);

  sleep(100);
  return 0;
};

};// namespace m3_tcp_io_test

#endif //SHE_NET_TEST_M3_TCP_IO_TEST_H_
