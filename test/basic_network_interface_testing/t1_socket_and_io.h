//
// Created by shecannotsee on 23-12-14.
//

#ifndef T1_SOCKET_AND_IO_H
#define T1_SOCKET_AND_IO_H

#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>
#include <thread>

namespace basic_network_interface_testing::t1_socket_and_io {

std::string main() {
  std::string test_return_message;

  /* tcp ipv4 */ {
    /* socket */
    const int tcp_ipv4_fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcp_ipv4_fd == -1) {
      test_return_message += "tcp_ipv4_fd socket create failed;\n";
    }
    /* port reuse */ {
      int reuse = 1;
      int setsockopt_result = ::setsockopt(tcp_ipv4_fd, SOL_SOCKET, SO_REUSEPORT, (const void *)&reuse, sizeof(reuse));
      if (setsockopt_result == -1) {
        test_return_message += "tcp_ipv4_fd set port reuse failed;\n";
      }
    }
    /* no block */ {
      int flags = fcntl(tcp_ipv4_fd, F_GETFL, 0);
      if (true) {
        flags |= O_NONBLOCK;
      } else {
        flags &= ~O_NONBLOCK;
      }
      int ret = fcntl(tcp_ipv4_fd, F_SETFL, flags);
      if (ret == -1) {
        test_return_message += "tcp_ipv4_fd set noblock failed;\n";
      }
    }
    /* bind */ {
      const std::string ip = "0.0.0.0";
      constexpr int port = 9000;
      struct sockaddr_in local_address {};
      ::memset(&local_address, 0, sizeof(local_address));
      local_address.sin_family      = AF_INET;
      local_address.sin_port        = htons(static_cast<unsigned short>(port));
      local_address.sin_addr.s_addr = ::inet_addr(ip.c_str());
      int bind_result               = ::bind(tcp_ipv4_fd, (struct sockaddr *)&local_address, sizeof(local_address));
      if (bind_result == -1) {
        test_return_message += "tcp_ipv4_fd bind 0.0.0.0:9000 failed;\n";
      }
    }
    /* listen */ {
      int listen_result = ::listen(tcp_ipv4_fd, 5);
      if (listen_result == -1) {
        test_return_message += "tcp_ipv4_fd listen failed;\n";
      }
    }
    int client_fd;
    bool server_start = false;
    std::thread client([&]() {
      while (server_start == false) {/* block */}
      const std::string ip = "0.0.0.0";
      constexpr int port = 9000;
      const int local_thread_fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      if (local_thread_fd == -1) {
        test_return_message += "tcp ipv4: client create socket failed;\n";
      }
      /* connect */ {
        struct sockaddr_in server_address {};
        ::memset(&server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;
        server_address.sin_port   = htons(static_cast<unsigned short>(port));
        ::inet_pton(AF_INET, ip.c_str(), &server_address.sin_addr);
        int connect_result = ::connect(local_thread_fd, (struct sockaddr *)&server_address, sizeof(server_address));
        if (connect_result == -1) {
          test_return_message += "tcp ipv4: client connect failed;\n";
        }
      }
      /* send message */ {
        for (int i = 1; i<4; i++) {
          std::this_thread::sleep_for(std::chrono::milliseconds(100));
          std::string msg = std::to_string(i);
          auto send_size = ::send(local_thread_fd, msg.data(), msg.size(), NULL);
          if (send_size == -1) {
            test_return_message += "tcp ipv4: client send msg failed;\n";
          }
        }
      }

      int close_result = ::shutdown(local_thread_fd, SHUT_RDWR);
      if (close_result == -1) {
        test_return_message += "tcp ipv4: client close fd failed;\n";
      }
    });
    /* accept and client thread */ {
      while (1) {
        struct sockaddr_in client_address {};
        socklen_t client_address_len = sizeof(client_address);
        int accept_result = ::accept(tcp_ipv4_fd, (struct sockaddr *)&client_address, &client_address_len);
        server_start = true;
        if (accept_result != -1) {
          client_fd = accept_result;
          break;
        }
      }
    }
    /* recv */ {
      int recv_num = 0;
      while (recv_num != (1+2+3)) {
        constexpr int buffer_size = 1024;
        char buffer[buffer_size];
        ssize_t bytes_read = ::recv(client_fd, buffer, buffer_size - 1, 0);
        if (bytes_read != -1 ) {
          buffer[bytes_read] = '\n';
          std::string str(buffer);
          int a = std::stoi(str);
          recv_num += a;
        }
      }
      std::cout << "recv success,jump to recv loop;\n";
    }
    /* close fd */ {
      int close_result = ::shutdown(tcp_ipv4_fd, SHUT_RDWR);
      if (close_result == -1) {
        test_return_message += "tcp ipv4 close fd failed;\n";
      }
    }
    client.join();
  }

  /* tcp ipv6 */ {
    /* socket */
    int tcp_ipv6_fd = ::socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    if (tcp_ipv6_fd == -1) {
      test_return_message += "tcp_ipv6 socket create failed;";
    }
    /* port reuse */ {
      int reuse = 1;
      int setsockopt_result = ::setsockopt(tcp_ipv6_fd, SOL_SOCKET, SO_REUSEPORT, (const void *)&reuse, sizeof(reuse));
      if (setsockopt_result == -1) {
        test_return_message += "tcp_ipv4 set port reuse failed;\n";
      }
    }
    /* no block */ {
      int flags = fcntl(tcp_ipv6_fd, F_GETFL, 0);
      if (true) {
        flags |= O_NONBLOCK;
      } else {
        flags &= ~O_NONBLOCK;
      }
      int ret = fcntl(tcp_ipv6_fd, F_SETFL, flags);
      if (ret == -1) {
        test_return_message += "tcp_ipv6_fd set noblock failed;\n";
      }
    }
    /* bind */ {
      const std::string ip = "::";
      constexpr int port = 9000;
      struct sockaddr_in6 local_address {};
      ::memset(&local_address, 0, sizeof(local_address));
      local_address.sin6_family = AF_INET6;
      local_address.sin6_port   = htons(static_cast<unsigned short>(port));
      ::inet_pton(AF_INET6, ip.c_str(), &local_address.sin6_addr);
      int bind_result               = ::bind(tcp_ipv6_fd, (struct sockaddr *)&local_address, sizeof(local_address));
      if (bind_result == -1) {
        test_return_message += "tcp_ipv6 bind :::9000 failed;\n";
      }
    }
    /* listen */ {
      int listen_result = ::listen(tcp_ipv6_fd, 5);
      if (listen_result == -1) {
        test_return_message += "tcp_ipv6_fd listen failed;\n";
      }
    }
  }

  /* udp ipv4 */ {
    /* socket */
    int upd_ipv4_fd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (upd_ipv4_fd == -1) {
      test_return_message += "upd_ipv4 socket create failed;";
    }
    /* port reuse */ {
      int reuse = 1;
      int setsockopt_result = ::setsockopt(upd_ipv4_fd, SOL_SOCKET, SO_REUSEPORT, (const void *)&reuse, sizeof(reuse));
      if (setsockopt_result == -1) {
        test_return_message += "upd_ipv4 set port reuse failed;\n";
      }
    }

  }

  /* udp ipv6 */ {
    /* socket */
    int udp_ipv6_fd = ::socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (udp_ipv6_fd == -1) {
      test_return_message += "udp_ipv6 socket create failed;";
    }
    /* port reuse */ {
      int reuse = 1;
      int setsockopt_result = ::setsockopt(udp_ipv6_fd, SOL_SOCKET, SO_REUSEPORT, (const void *)&reuse, sizeof(reuse));
      if (setsockopt_result == -1) {
        test_return_message += "udp_ipv6_fd set port reuse failed;\n";
      }
    }
  }

  return test_return_message.empty()?"t1_socket_and_io pass test":test_return_message;
}

}



#endif //T1_SOCKET_AND_IO_H
