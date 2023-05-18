//
// Created by shecannotsee on 2023/3/10.
//

#ifndef SHE_NET_TEST_M5_SELECT_TEST_H_
#define SHE_NET_TEST_M5_SELECT_TEST_H_

#include <string>
#include <vector>
#include <mutex>
#include "high_coupling_model/socket.h"
#include <sys/select.h>
#include <memory>
#include <iostream>
#include <unistd.h>
#include <future>
#include <sys/socket.h>
#include "high_coupling_model/message.h"
#include "make_unique.h"

namespace m5_select_test {

int main() {
  std::string ip = "192.168.199.129";
  std::string port = "9981";

  sheNet::socket server_socket; /* set */ {
    server_socket.bind(port);
    server_socket.listen();
  };

  static std::vector<int> client_fds; // 存储客户端套接字的文件描述符
  fd_set read_fds;    /* 读事件 */ {
    /* 将所有bit置为0 */
    FD_ZERO(&read_fds);
  };
  fd_set write_fds;   /* 写事件 */ {
    FD_ZERO(&write_fds);
  };
  fd_set except_fds;  /* 异常事件 */ {
    FD_ZERO(&except_fds);
  };
  int max_fd = 1024; // 最大文件描述符

  auto client = [&](std::string data) {
    try {
      sheNet::TRANSPORT_ADDRESS_TYPE tcp = sheNet::TRANSPORT_ADDRESS_TYPE::TCP_IPV4;
      std::unique_ptr<sheNet::socket> client = sheNet::CPP11::make_unique<sheNet::socket>(tcp);/* tcp init */ {
        client->connect(ip, port);
      };
      sheNet::message message_control(std::move(client));
      std::cout << "client set done.\n";
      int i = 3;
      while (i--) {
        message_control.send("hello world"+data);
        std::cout << "send done." + data <<"\n";
        sleep(2);
      }
    } catch (const std::exception& exc) {
      std::cout<<exc.what()<<std::endl;
    }
  };
  auto client_future1 = std::async(std::launch::async,client,"1");
  auto client_future2 = std::async(std::launch::async,client,"2");
  auto client_future3 = std::async(std::launch::async,client,"3");
  auto client_future4 = std::async(std::launch::async,client,"4");

  std::mutex mtx;
  auto accept_thread = [&]() {
    int client_num = 0;
    while (client_num != 4) {
      std::unique_lock<std::mutex> lock(mtx);
      server_socket.accept();
      std::cout << "new connection from " << server_socket.get_destination_ip()
                << ":" << server_socket.get_destination_port()
                << ":" << server_socket.get_destination_id()
                << std::endl;
      client_fds.push_back(server_socket.get_destination_id());
      // client fd加入事件集合
      FD_SET(server_socket.get_destination_id(), &read_fds);
      client_num++;
    }
  };
  auto accept_result = std::async(std::launch::async,accept_thread);

  while (1) {
    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "start while\n";
    FD_ZERO(&read_fds);
    // 将server fd加入事件集合
    FD_SET(server_socket.get_source_id(), &read_fds);
    for (auto client_fd : client_fds) {
      FD_SET(client_fd, &read_fds);
    }

    struct timeval tTime;
    tTime.tv_sec = 10;
    tTime.tv_usec = 0;
    // 等待事件发生,实际上是将触发io操作的fd的bit位置1
    int ret = ::select(max_fd + 1, &read_fds, nullptr, nullptr, &tTime);
    if (ret == -1) {
      perror("select");
      break;
    };

    // 会遍历所有添加的fd
    for (auto client_fd : client_fds) {
      // fd is ready to io,若该fd被置为1表示触发了io操作,才会去进行io操作
      bool result = FD_ISSET(client_fd, &read_fds);
      if (result) {
        // 有客户端套接字的数据可读
        char buf[1024];
        int n = recv(client_fd, buf, sizeof(buf), 0);
        if (n <= 0) {
          // 客户端已经关闭连接
          std::cout << "client disconnected" << std::endl;
          close(client_fd);
          FD_CLR(client_fd,&read_fds);
        } else {
          std::cout << "recv " << n << " bytes from client" << std::endl;
        }
      }
    }

  };// while(1)



  return 0;
};

};// namespace m5_select_test

#endif //SHE_NET_TEST_M5_SELECT_TEST_H_
