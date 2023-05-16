//
// Created by shecannotsee on 23-5-15.
//

#include "basic_io_operations.h"

#include <string.h>
#include <sys/socket.h>
#include <errno.h>

#include <sheNetException/sheNetException.h>

std::string sheNet::basic_io_operations::recv(int fd) {
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

void sheNet::basic_io_operations::send(int fd, const std::string &binary_stream) {
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
