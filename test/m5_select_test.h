//
// Created by shecannotsee on 2023/3/10.
//

#ifndef SHE_NET_TEST_M5_SELECT_TEST_H_
#define SHE_NET_TEST_M5_SELECT_TEST_H_

#include <string>
#include <socket.h>
#include <sys/select.h>

namespace m5_select_test {

int main() {
  std::string ip = "192.168.1.47";
  std::string port = "9981";

  sheNet::socket server_socket; /* set */ {
    server_socket.bind(port);
    server_socket.listen();
  };

  fd_set read_fds;
  fd_set write_fds;
  fd_set except_fds;
  FD_ZERO(&read_fds);
  FD_ZERO(&write_fds);
  FD_ZERO(&except_fds);

  while (1) {
    std::string buffer;
    buffer.resize(1024);
    fd_set tmp_read_fds = read_fds;
    fd_set tmp_write_fds = write_fds;
    fd_set tmp_except_fds = except_fds;
    struct timeval timeout = {0,0};
    int ready_fds = select(2047,&tmp_read_fds, &tmp_write_fds, &tmp_except_fds, /*block:NULL*//*noblock:{0,0}*/&timeout);
    if (ready_fds == -1) {
      std::cerr << "Failed to select sockets." << std::endl;
      break;
    }

    for (int fd = 0; fd <= 2048 && ready_fds > 0; ++fd) {
      if (FD_ISSET(fd, &tmp_read_fds)) {
        --ready_fds;
        if (fd == server_socket.get_source_id()) {
          // New client connection
          sockaddr_in client_address;
          socklen_t client_address_len = sizeof(client_address);
          int client_socket = accept(server_socket, reinterpret_cast<sockaddr*>(&client_address), &client_address_len);
          if (client_socket == -1) {
            std::cerr << "Failed to accept client connection." << std::endl;
          } else {
            std::cout << "New client connected." << std::endl;
            FD_SET(client_socket, &read_fds);
            max_fd = std::max(max_fd, client_socket);
          }
        } else {
          // Client message
          char buffer[BUFFER_SIZE];
          ssize_t len = recv(fd, buffer, BUFFER_SIZE, 0);
          if (len <= 0) {
            // Client disconnected
            std::cout << "Client disconnected." << std::endl;
            FD_CLR(fd, &read_fds);
            close(fd);
          } else {
            // Echo message back to client
            buffer[len] = '\0';
            std::cout << "Received message: " << buffer << std::endl;
            FD_SET(fd, &write_fds);
          }
        }
      }
      else {
      }
    }


  }

  return 0;
};

};// namespace m5_select_test

#endif //SHE_NET_TEST_M5_SELECT_TEST_H_
