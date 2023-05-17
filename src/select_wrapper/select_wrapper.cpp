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

sheNet::select_wrapper::select_wrapper()
    : timeout_set_({}) {
  FD_ZERO(&read_fds_);
  FD_ZERO(&write_fds_);
  FD_ZERO(&except_fds_);
  // 只增大其容量到最大监听描述符+1
  fd_list_.reserve(sheNet::select_wrapper::get_system_max_listen_fd() +1 );
  // 超时设置为0,也就是非阻塞
  timeout_set_.tv_sec  = 0;
  timeout_set_.tv_usec = 0;
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

void sheNet::select_wrapper::set_timeout(int seconds, int microseconds) {
  timeout_set_.tv_sec  = seconds;
  timeout_set_.tv_usec = microseconds;
};

std::vector<int> sheNet::select_wrapper::get_alive_fd() {
  int num_events = ::select(this->get_system_max_listen_fd()+1,
                               &read_fds_,
                               &write_fds_,
                               &except_fds_,
                               &timeout_set_);
  /*z*/if (num_events == -1) {
    throw sheNetException(15,"select system interface error:"+std::string(strerror(errno)));
  }
  else if (num_events == 0) {
    return {};
  }
  else {
    std::vector<int> alive_fds;
    for (const auto &fd: fd_list_) {
      bool is_read = FD_ISSET(fd, &read_fds_);
      if (is_read) {
        alive_fds.push_back(fd);
      }
    }
    return alive_fds;
  }

};







