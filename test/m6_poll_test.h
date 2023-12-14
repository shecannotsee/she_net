//
// Created by shecannotsee on 2023/3/10.
//

#ifndef SHE_NET_TEST_M6_POLL_TEST_H_
#define SHE_NET_TEST_M6_POLL_TEST_H_

#include <string>
#include <vector>
#include "high_coupling_model/socket.h"
#include <sys/poll.h>

namespace m6_poll_test {

int main() {
  std::string ip = "192.168.1.47";
  std::string port = "9981";

  she_net::socket server_socket; /* set */ {
    server_socket.bind(port);
    server_socket.listen();
  };

  std::vector<pollfd> poll_fds;
  // 添加server fd
  poll_fds.push_back({server_socket.get_source_id(),POLLIN, 0});

  auto client = [&](std::string data) {
    try {
      she_net::TRANSPORT_ADDRESS_TYPE tcp = she_net::TRANSPORT_ADDRESS_TYPE::TCP_IPV4;
      std::unique_ptr<she_net::socket> client = she_net::CPP11::make_unique<she_net::socket>(tcp);/* tcp init */ {
        client->connect(ip, port);
      };
      she_net::message message_control(std::move(client));
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

  while (1) {
    int ret = poll(poll_fds.data(), poll_fds.size(), 5);
    if (ret < 0) {
      std::cerr << "poll error: " << std::strerror(errno) << std::endl;
      break;
    }

    // 连接上的client添加,也就是在检查server fd是否被poll接口改为POLLIN,如果被修改表示触发了io操作(其实这里的io操作就是有客户端在连接)
    if (poll_fds[0].revents & POLLIN) {
      // 有新连接请求
      server_socket.accept();
      std::cout << "new connection from " << server_socket.get_destination_ip()
                << ":" << server_socket.get_destination_port()
                << ":" << server_socket.get_destination_id()
                << std::endl;
      poll_fds.push_back({server_socket.get_destination_id(), POLLIN, 0});
    }

    // 遍历除server fd之外的所有fd
    for (int i = 0; i < poll_fds.size() ; i++) {
      // 仍然要遍历所有已经添加的fd,触发io操作的fd(revents被置为POLLIN的表示触发了io操作,该变量的修改会在poll接口里由内核去做)
      if ((poll_fds[i].revents & POLLIN) && poll_fds[i].fd != server_socket.get_source_id()) { // 该fd可读
        char buf[1024];
        int n = recv(poll_fds[i].fd, buf, sizeof(buf), 0);
        if (n == -1) {
          perror("recv");
          close(poll_fds[i].fd);
          poll_fds.erase(poll_fds.begin() + i); // 将该fd从pollfd数组中移除
          continue;
        }
        if (n == 0) { // 客户端关闭连接
          std::cout << "client disconnected" << std::endl;
          close(poll_fds[i].fd);
          poll_fds.erase(poll_fds.begin() + i); // 将该fd从pollfd数组中移除
          continue;
        }
        std::cout << "recv " << n << " bytes from client" << std::endl;
        poll_fds[i].events |= POLLOUT; // 修改该fd的事件为可读可写
      }
    }

  };// while(1)

  return 0;
};

};// namespace m6_poll_test

#endif //SHE_NET_TEST_M6_POLL_TEST_H_
