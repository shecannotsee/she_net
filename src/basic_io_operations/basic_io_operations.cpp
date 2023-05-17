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
  constexpr int bufferSize = 1024;
  char buffer[bufferSize];

  ssize_t bytesRead = ::recv(fd, buffer, bufferSize - 1, 0);
  if (bytesRead == -1) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      // 没有更多可用数据
      return {};
    } else {
      throw sheNetException(6, "tcp recv data error." + std::string(strerror(errno)));
    }
  } else if (bytesRead == 0) {
    // 客户端关闭
    throw sheNetException(12, "tcp recv error: The client has been shut down.");
  } else {
    // 数据成功抵达
    buffer[bytesRead] = '\0'; // Null-terminate the received data
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
  std::string buffer;
  // 注意,该初始化不仅会扩大其容量,并且会对开辟的内存进行相应的类初始化
  buffer.resize(1024);
  bool accepting = true;
  struct sockaddr_in senderAddr;
  socklen_t senderAddrLen = sizeof(senderAddr);
  while (accepting) {
    // TODO:若超过1024字节,需要重新调整buff的传入指针
    ssize_t number_of_bytes_accepted =
        ::recvfrom(fd, &buffer[0], buffer.size(), NULL, (struct sockaddr *) &senderAddr, &senderAddrLen);
    if (number_of_bytes_accepted == -1) {
      if (errno == EINTR) {
        // 如果被信号中断则重试
        continue;
      } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        // 如果是非阻塞的，则等待一段时间后重试
        accepting = false;
      } else {
        // 出现其他错误，需要处理
        throw sheNetException(8, "upd recv data error." + std::string(strerror(errno)));
      }
    } else if(number_of_bytes_accepted == 0) {
      // 如果连接关闭(local fd关闭)则返回空字符串
      throw sheNetException(13, "udp recvfrom error: The local fd has been shut down.");
    } else {
      // TODO:成功读取了n个字节的数据之后,可能数据仍然不完整,需要做缓冲并且将流数据进行切割处理
      accepting = false;
    }
  };
  return buffer;
};

void sheNet::basic_io_operations::UDP::sendto(int fd, std::string ip, std::string port, std::string binary_package, TRANSPORT_ADDRESS_TYPE type) {
  int result = 0;//已经添加到缓冲区的字节长度
  // 当所有数据添加到发送缓冲区时可是为发送成功
  while (result == binary_package.size()) {
    int number_of_bytes_send = -1;
    /*z*/if (type == TRANSPORT_ADDRESS_TYPE::UDP_IPV4) {
      struct sockaddr_in server_address{};
      ::memset(&server_address, 0x00, sizeof(server_address));
      server_address.sin_family = AF_INET;
      server_address.sin_addr.s_addr = ::inet_addr(ip.c_str());
      server_address.sin_port = htons(std::atoi(port.c_str()));

      number_of_bytes_send = ::sendto(fd,
                        binary_package.c_str() + result, // 发送指针向后移动
                        binary_package.size() - result,    // 发送数据减小
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

      number_of_bytes_send = ::sendto(fd,
                        binary_package.c_str() + result,
                        binary_package.size() - result,
                        NULL,
                        (struct sockaddr *) &server_address,
                        sizeof(server_address));
    }
    else {
      assert(false);
    }

    if (number_of_bytes_send == -1) {
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
    } else if (number_of_bytes_send < binary_package.size()) {
      // 添加发送的字节数
      result += number_of_bytes_send;
    } else if (number_of_bytes_send == binary_package.size()) {
      // 成功全部发送
      // Do nothing.
    }
  }
}
