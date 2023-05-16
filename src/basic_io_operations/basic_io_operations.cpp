//
// Created by shecannotsee on 23-5-15.
//

#include "basic_io_operations.h"

#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>

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
}
