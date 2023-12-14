//
// Created by shecannotsee on 23-5-17.
//

#include "poll_wrapper.h"

#include <errno.h>
#include <she_net_exception/she_net_exception.h>
#include <string.h>

#include <algorithm>
#include <string>

she_net::poll_wrapper::poll_wrapper() : timeout_set_(0){};

void she_net::poll_wrapper::add_server_fd(int local_fd) {
  poll_fds_.push_back({local_fd, POLLIN, 0});
};

void she_net::poll_wrapper::add_alive_fd(int fd) {
  poll_fds_.push_back({fd, POLLIN, 0});
};

void she_net::poll_wrapper::remove_alive_fd(int fd) {
  poll_fds_.erase(std::remove_if(poll_fds_.begin(), poll_fds_.end(), [fd](const pollfd& pfd) { return pfd.fd == fd; }),
                  poll_fds_.end());
};

void she_net::poll_wrapper::set_timeout(int seconds) {
  timeout_set_ = seconds;
};

std::vector<int> she_net::poll_wrapper::get_alive_fd() {
  int num_events = ::poll(poll_fds_.data(), poll_fds_.size(), timeout_set_);
  if (num_events == -1) {
    throw she_net_exception(16, "poll system interface error:" + std::string(strerror(errno)));
  } else if (num_events == 0) {
    return {};
  } else {
    std::vector<int> alive_fds;
    // 遍历除server fd之外的所有fd,所以从1开始
    for (int i = 1; i < poll_fds_.size(); i++) {
      // 仍然要遍历所有已经添加的fd,触发io操作的fd(revents被置为POLLIN的表示触发了io操作,该变量的修改会在poll接口里由内核去做)
      if ((poll_fds_[i].revents & POLLIN)) {  // 该fd可读
        poll_fds_[i].events |= POLLOUT;  // 修改该fd的事件为可读可写,以便于下次事件触发后可以继续进行处理
        alive_fds.push_back(poll_fds_[i].fd);
      }
    }
    return alive_fds;
  }
};
