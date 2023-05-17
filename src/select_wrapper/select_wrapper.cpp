//
// Created by shecannotsee on 2023/3/9.
//

#include "select_wrapper.h"

#include <string.h>
#include <string>
#include <algorithm>

#include <sys/socket.h>
#include <sys/resource.h>

#include <sheNetException/sheNetException.h>

sheNet::select_wrapper::select_wrapper() {
  FD_ZERO(&read_fds_);
  FD_ZERO(&write_fds_);
  FD_ZERO(&except_fds_);
  // 只增大其容量到最大监听描述符+1
  fd_list_.reserve(sheNet::select_wrapper::get_system_max_listen_fd() +1 );
};

int sheNet::select_wrapper::get_system_max_listen_fd() {
  rlimit rlim{};
  if (getrlimit(RLIMIT_NOFILE, &rlim) == 0) {
    return static_cast<int>(rlim.rlim_cur);
  } else {
    throw sheNetException(14,"get select max value error:"+std::string(strerror(errno)));
  }
};

void sheNet::select_wrapper::add_alive_fd(int fd) {
  fd_list_.push_back(fd);
  FD_SET(fd, &read_fds_);
};

void sheNet::select_wrapper::remove_alive_fd(int fd) {
  auto it = std::remove(fd_list_.begin(), fd_list_.end(), fd);
  fd_list_.erase(it);
};





