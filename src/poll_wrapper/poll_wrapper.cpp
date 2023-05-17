//
// Created by shecannotsee on 23-5-17.
//

#include "poll_wrapper.h"
#include <algorithm>

sheNet::poll_wrapper::poll_wrapper() = default;

void sheNet::poll_wrapper::add_server_fd(int local_fd) {
  poll_fds_.push_back({
    local_fd,
    POLLIN,
    0
  });
};

void sheNet::poll_wrapper::add_alive_fd(int fd) {
  poll_fds_.push_back({
    fd,
    POLLIN,
    0
  });
};

void sheNet::poll_wrapper::remove_alive_fd(int fd) {
  poll_fds_.erase(std::remove_if(poll_fds_.begin(), poll_fds_.end(), [fd](const pollfd& pfd) {
    return pfd.fd == fd;
  }), poll_fds_.end());
};
