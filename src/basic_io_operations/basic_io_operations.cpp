//
// Created by shecannotsee on 23-5-15.
//

#include "basic_io_operations.h"

#include <assert.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sheNetException/sheNetException.h>

std::string sheNet::basic_io_operations::TCP::recv(int fd) {
  constexpr int buffer_size = 1024;
  char buffer[buffer_size];

  ssize_t bytes_read = ::recv(fd, buffer, buffer_size - 1, 0);
  if (bytes_read == -1) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      // 没有更多可用数据
      return {};
    } else {
      throw sheNetException(6, "tcp recv data error." + std::string(strerror(errno)));
    }
  } else if (bytes_read == 0) {
    // 客户端关闭
    throw sheNetException(12, "tcp recv error: The client has been shut down.");
  } else {
    // 数据成功抵达
    buffer[bytes_read] = '\0'; // Null-terminate the received data
    return std::string(buffer);
  }
};

void sheNet::basic_io_operations::TCP::send(int fd, const std::string &binary_stream) {
  size_t total_sent = 0; // 已发送字节数
  while (total_sent < binary_stream.size()) {
    ssize_t send_bytes = ::send(fd, binary_stream.data() + total_sent, binary_stream.size() - total_sent, NULL);
    if (send_bytes == -1) {
      if (errno == EINTR) {
        // 被信号打断了，继续发送
        continue;
      } else if (errno == EINTR|| errno == EWOULDBLOCK) {
        // TODO:需要处理缓冲区满的情况或者阻塞
        continue;
      } else {
        throw sheNetException(7, "tcp send message error." + std::string(strerror(errno)));
      }
    }
    total_sent += send_bytes;
  }
};

std::string sheNet::basic_io_operations::UDP::recvfrom(int fd) {
  constexpr int buffer_size = 1024;
  char buffer[buffer_size];
  struct sockaddr_in senderAddr;
  socklen_t senderAddrLen = sizeof(senderAddr);
  ssize_t bytes_read =
      ::recvfrom(fd, buffer, buffer_size-1, NULL, (struct sockaddr *) &senderAddr, &senderAddrLen);
  /*z*/if (bytes_read == -1) {
    if (errno == EINTR) {
      return {};
    } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
      // 如果是非阻塞的，则等待一段时间后重试
      return {};
    } else {
      // 出现其他错误，需要处理
      throw sheNetException(8, "upd recv data error." + std::string(strerror(errno)));
    }
  }
  else if (bytes_read == 0) {
    // 如果连接关闭(local fd关闭)则返回空字符串
    throw sheNetException(13, "udp recvfrom error: The local fd has been shut down.");
  }
  else {
    buffer[bytes_read] = '\0'; // Null-terminate the received data
    return std::string(buffer);
  }
};

void sheNet::basic_io_operations::UDP::sendto(int fd, std::string ip, std::string port, std::string binary_package, TRANSPORT_ADDRESS_TYPE type) {
  int bytes_sent = -1;
  for (bool loop = true; loop ; ) {
    /*z*/if (type == TRANSPORT_ADDRESS_TYPE::UDP_IPV4) {
      struct sockaddr_in server_address{};
      ::memset(&server_address, 0x00, sizeof(server_address));
      server_address.sin_family = AF_INET;
      server_address.sin_addr.s_addr = ::inet_addr(ip.c_str());
      server_address.sin_port = htons(std::atoi(port.c_str()));

      bytes_sent = ::sendto(fd,
                            binary_package.c_str(),
                            binary_package.size(),
                            NULL,
                            (struct sockaddr *) &server_address,
                            sizeof(server_address));
    }
    else if (type == TRANSPORT_ADDRESS_TYPE::UDP_IPV6) {
      struct sockaddr_in6 server_address{};
      ::memset(&server_address, 0x00, sizeof(server_address));
      server_address.sin6_family = AF_INET6;
      ::inet_pton(AF_INET6, ip.c_str(), &server_address.sin6_addr);
      server_address.sin6_port = htons(std::atoi(port.c_str()));

      bytes_sent = ::sendto(fd,
                            binary_package.c_str(),
                            binary_package.size(),
                            NULL,
                            (struct sockaddr *) &server_address,
                            sizeof(server_address));
    }
    else {
      assert(false);
    }

    if (bytes_sent == -1) {
      // 对非阻塞进行的额外操作
      if (errno == EINTR) {
        // 发送操作被信号中断，重新尝试发送
        continue;
      } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        // 发送缓冲区已满，稍后再次尝试发送
        continue;
      } else {
        throw sheNetException(9, "upd send message error." + std::string(strerror(errno)));
      }
    }

    if ( bytes_sent == binary_package.size() ) {
      loop == false;
    }
  }
}
