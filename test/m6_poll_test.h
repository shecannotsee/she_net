//
// Created by shecannotsee on 2023/3/10.
//

#ifndef SHE_NET_TEST_M6_POLL_TEST_H_
#define SHE_NET_TEST_M6_POLL_TEST_H_

#include <string>
#include <vector>
#include <socket.h>
#include <sys/poll.h>

namespace m6_poll_test {

int main() {
  std::string ip = "192.168.199.129";
  std::string port = "9981";

  sheNet::socket server_socket; /* set */ {
    server_socket.bind(port);
    server_socket.listen();
  };

  std::vector<pollfd> poll_fds;
  // 添加server fd
  poll_fds.push_back({server_socket.get_source_id(),POLLIN, 0});

  while (1) {
    int ret = poll(poll_fds.data(), poll_fds.size(), -1);
    if (ret < 0) {
      std::cerr << "poll error: " << std::strerror(errno) << std::endl;
      break;
    }

    // 连接上的client添加
    if (poll_fds[0].revents & POLLIN) {
      // 有新连接请求
      int client_fd = accept(server_socket.get_source_id(), nullptr, nullptr);
      if (client_fd < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
          // 已经处理完所有连接请求
        } else {
          std::cerr << "accept error: " << std::strerror(errno) << std::endl;
          break;
        }
      } else {
        std::cout << "new connection accepted: " << client_fd << std::endl;
        // 将新连接的socket加入pollfd数组
        poll_fds.push_back({client_fd, POLLIN, 0});
      }
    }

    // 遍历除server fd之外的所有fd
    for (int i = 0; i < poll_fds.size() ; i++) {
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
