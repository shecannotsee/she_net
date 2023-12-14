//
// Created by shecannotsee on 2023/3/9.
//

#include "select_wrapper.h"

#include <she_net_exception/she_net_exception.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/socket.h>

#include <algorithm>
#include <string>

she_net::select_wrapper::select_wrapper() : timeout_set_({}) {
  FD_ZERO(&read_fds_);
  FD_ZERO(&write_fds_);
  FD_ZERO(&except_fds_);
  // 只增大其容量到最大监听描述符+1
  fd_list_.reserve(she_net::select_wrapper::get_system_max_listen_fd() + 1);
  // 超时设置为0,也就是非阻塞
  timeout_set_.tv_sec  = 0;
  timeout_set_.tv_usec = 0;
};

int she_net::select_wrapper::get_system_max_listen_fd() {
  rlimit rlim{};
  if (getrlimit(RLIMIT_NOFILE, &rlim) == 0) {
    return static_cast<int>(rlim.rlim_cur);
  } else {
    throw she_net_exception(14, "get select max value error:" + std::string(strerror(errno)));
  }
};

void she_net::select_wrapper::add_alive_fd(int fd) {
  fd_list_.push_back(fd);
  FD_SET(fd, &read_fds_);
};

void she_net::select_wrapper::remove_alive_fd(int fd) {
  auto it = std::remove(fd_list_.begin(), fd_list_.end(), fd);
  fd_list_.erase(it);
};

void she_net::select_wrapper::set_timeout(int seconds, int microseconds) {
  timeout_set_.tv_sec  = seconds;
  timeout_set_.tv_usec = microseconds;
};

std::vector<int> she_net::select_wrapper::get_alive_fd(int local_fd) {
  // 在每次调用select之前都要重置其监听列表
  FD_ZERO(&read_fds_);
  FD_SET(local_fd, &read_fds_);  // 添加服务端fd
  // 添加已经通过accept连接上服务端的客户端fd
  for (auto fd : fd_list_) {
    FD_SET(fd, &read_fds_);
  }

  // start to call ::select()
  int num_events = ::select(this->get_system_max_listen_fd() + 1, &read_fds_, &write_fds_, &except_fds_, &timeout_set_);
  /*zz*/ if (num_events == -1) {
    throw she_net_exception(15, "select system interface error:" + std::string(strerror(errno)));
  } else if (num_events == 0) {
    return {};
  } else {
    std::vector<int> alive_fds;
    for (const auto &fd : fd_list_) {
      bool is_read = FD_ISSET(fd, &read_fds_);
      if (is_read) {
        alive_fds.push_back(fd);
      }
    }
    return alive_fds;
  }
};
