//
// Created by shecannotsee on 2023/3/10.
//

#ifndef SHE_NET_TEST_M5_SELECT_TEST_H_
#define SHE_NET_TEST_M5_SELECT_TEST_H_

#include <string>
#include <vector>
#include <socket.h>
#include <sys/select.h>

namespace m5_select_test {

int main() {
  std::string ip = "192.168.199.129";
  std::string port = "9981";

  sheNet::socket server_socket; /* set */ {
    server_socket.bind(port);
    server_socket.listen();
  };

  std::vector<int> client_fds; // 存储客户端套接字的文件描述符
  fd_set read_fds; // 读事件集合
  FD_ZERO(&read_fds);
  int max_fd = 1024; // 最大文件描述符

  auto client = [&](std::string data) {
    try {
      sheNet::NetTransport tcp = sheNet::NetTransport::TCP_IPV4;
      std::unique_ptr<sheNet::socket> client = sheNet::CPP11::make_unique<sheNet::socket>(tcp);/* tcp init */ {
        client->connect(ip, port);
      };
      sheNet::message message_control(std::move(client));
      std::cout << "client set done.\n";
      while (1) {
        message_control.send("hello world"+data);
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

  while (1) {
    // 设置事件集合
    FD_SET(server_socket.get_source_id(), &read_fds);
    for (auto fd : client_fds) {
      FD_SET(fd, &read_fds);
      if (fd > max_fd) {
        max_fd = fd;
      }
    }

    // 等待事件发生
    int ret = ::select(max_fd + 1, &read_fds, nullptr, nullptr, nullptr);
    if (ret == -1) {
      perror("select");
      break;
    };

    // 处理事件
    if (FD_ISSET(server_socket.get_source_id(), &read_fds)) {
      // 有新的客户端连接请求
      server_socket.accept();
      std::cout << "new connection from "<< server_socket.get_destination_ip()
                << ":" << server_socket.get_destination_port()
                << std::endl;
      client_fds.push_back(server_socket.get_destination_id());
    }

    for (auto iter = client_fds.begin(); iter != client_fds.end();) {
      int client_fd = *iter;
      if (FD_ISSET(client_fd, &read_fds)) {
        // 有客户端套接字的数据可读
        char buf[1024];
        int n = recv(client_fd, buf, sizeof(buf), 0);
        if (n <= 0) {
          // 客户端已经关闭连接
          std::cout << "client disconnected" << std::endl;
          close(client_fd);
          iter = client_fds.erase(iter);
          continue;
        }
        std::cout << "recv " << n << " bytes from client" << std::endl;
      }
      ++iter;
    }

    // 清空事件集合
    FD_ZERO(&read_fds);

  };// while(1)



  return 0;
};

};// namespace m5_select_test

#endif //SHE_NET_TEST_M5_SELECT_TEST_H_
