//
// Created by shecannotsee on 2023/3/30.
//

#ifndef SHE_NET_TEST_M8_TCP_BIDIRECTIONAL_IO_TEST_H_
#define SHE_NET_TEST_M8_TCP_BIDIRECTIONAL_IO_TEST_H_

#include <iostream>
#include <string>
#include <future>
#include <memory>

#include <socket.h>
#include <message.h>
#include <net_transport.h>
#include <make_unique.h>

#include <unistd.h>

namespace m8_tcp_bidirectional_io_test {

int main() {
  auto server_recv_and_send = [](const std::string& local_port,const std::string& connect_port,int flag) {
    try {
      sheNet::TRANSPORT_ADDRESS_TYPE tcp = sheNet::TRANSPORT_ADDRESS_TYPE::TCP_IPV4;
      std::unique_ptr<sheNet::socket> server = sheNet::CPP11::make_unique<sheNet::socket>(tcp);
      std::unique_ptr<sheNet::socket> client = sheNet::CPP11::make_unique<sheNet::socket>(tcp);
      // 由于accept和connect均为阻塞，所以在两个server出现的时候需要有一个先启服务，然后等待另一个进行连接，这样才能顺利的进行下去
      // 这里也是不太想使用异步来处理,因为异步嵌套已经比较深了所以做了一个顺序先后，保证先监听再连接或者先连接再监听
      if (flag==0) {
        server->bind(local_port);
        server->listen();
        server->accept();
        client->connect("192.168.1.47",connect_port);
      } else if (flag == 1) {
        client->connect("192.168.1.47",connect_port);
        server->bind(local_port);
        server->listen();
        server->accept();
      }

      sheNet::message message_control_server(std::move(server));
      sheNet::message message_control_client(std::move(client));

      auto send_thread = std::async(std::launch::async,[&](){
        while (1) {
          message_control_client.send("1");
          std::cout<<"send to "<<connect_port << "success.\n";
          sleep(1);
        }
      });
      auto recv_thread = std::async(std::launch::async,[&](){
        while (1) {
          std::string data = message_control_server.get();
          std::cout << "data is:[" << data << "]\n";
          sleep(1);
        }
      });
    } catch (const std::exception& exc) {
      std::cout<<exc.what()<<std::endl;
    }
  };

  // 绑定本机的9981端口进行监听;去连接9982端口,然后向该端口发送数据
  auto server_future = std::async(std::launch::async,server_recv_and_send,"9981","9982",0);
  // 绑定本机的9982端口进行监听;去连接9981端口,然后向该端口发送数据
  auto client_future = std::async(std::launch::async,server_recv_and_send,"9982","9981",1);
//  sleep(100);
  return 0;
};

};// m8_tcp_bidirectional_io_test

#endif //SHE_NET_TEST_M8_TCP_BIDIRECTIONAL_IO_TEST_H_
