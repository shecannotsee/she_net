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
      // No more data available, continue or exit the loop
      return {};
    } else {
      throw sheNetException(6, "tcp recv data error." + std::string(strerror(errno)));
    }
  } else if (bytesRead == 0) {
    // Connection closed by the other end
    // Handle the connection closed scenario here
    throw sheNetException(12, "tcp recv error: The client has been shut down.");
  } else {
    // Data received successfully
    buffer[bytesRead] = '\0'; // Null-terminate the received data
    return std::string(buffer);
  }
};
