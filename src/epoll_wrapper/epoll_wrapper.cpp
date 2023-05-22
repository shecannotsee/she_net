//
// Created by shecannotsee on 23-5-17.
//

#include "epoll_wrapper.h"

#include <errno.h>
#include <string.h>
#include <string>

#include <sys/epoll.h>

#include <sheNetException/sheNetException.h>

sheNet::epoll_wrapper::epoll_wrapper()
    : epoll_wrapper(1024) {};

sheNet::epoll_wrapper::epoll_wrapper(int events_num)
    : user_events_(events_num),
      timeout_set_(-1),
      server_socket_fd_(-1) {
  epoll_container_id_ = ::epoll_create(1);
};

void sheNet::epoll_wrapper::add_server_fd(int local_fd) {
  server_socket_fd_ = local_fd;
  epoll_event event_options{};
  event_options.events  = EPOLLIN ;
  event_options.data.fd = server_socket_fd_;
  int control_result = ::epoll_ctl(epoll_container_id_,EPOLL_CTL_ADD,server_socket_fd_,&event_options);
  if (control_result == -1) {
    throw sheNetException(99,"epoll_ctl add system interface error:"+std::string(strerror(errno)));
  }
};

void sheNet::epoll_wrapper::add_alive_fd(int fd ,EPOLL_EVENTS trigger_mode) const {
  epoll_event event_options{};
  event_options.events  = EPOLLIN | trigger_mode;
  event_options.data.fd = fd;
  int control_result = ::epoll_ctl(epoll_container_id_,EPOLL_CTL_ADD,fd,&event_options);
  if (control_result == -1) {
    throw sheNetException(17,"epoll_ctl add system interface error:"+std::string(strerror(errno)));
  }
};

void sheNet::epoll_wrapper::remove_alive_fd(int fd) {
  epoll_event event_options{};
  event_options.data.fd = fd;

  int control_result =
          ::epoll_ctl(epoll_container_id_, EPOLL_CTL_DEL, server_socket_fd_, &event_options);
  // 调用 epoll_ctl，使用 EPOLL_CTL_DEL 操作将客户端的文件描述符从 epoll 实例中剔除
  if (control_result == -1) {
    throw sheNetException(18,"epoll_ctl add system interface error:"+std::string(strerror(errno)));
  }
}

void sheNet::epoll_wrapper::set_timeout(int milliseconds) {
  timeout_set_ = milliseconds;
};

int sheNet::epoll_wrapper::accept_alive() {
  int num_events = ::epoll_wait(epoll_container_id_,user_events_.data(),user_events_.size(),timeout_set_);
  if (num_events == -1) {
    // 不要特殊去处理errno为EINTR的情况,因为在信号中断的情况下可能会出现内核清除已注册的事件的事情发生
    throw sheNetException(19,"epoll_wait(accept) system interface error:"+std::string(strerror(errno)));
  } else if (num_events == 0) {
    return -1;
  } else {
    for (int i = 0; i <num_events; ++i) {
      // 可读事件,并且该事件是accept的时候返回
      if (user_events_[i].events == EPOLLIN && (user_events_[i].data.fd == server_socket_fd_)) {
        return user_events_[i].data.fd;
      }
    }
    return -1;
  }
};

std::vector<int> sheNet::epoll_wrapper::get_alive_fd() {
  int num_events = ::epoll_wait(epoll_container_id_,user_events_.data(),user_events_.size(),timeout_set_);
  if (num_events == -1) {
    // 不要特殊去处理errno为EINTR的情况,因为在信号中断的情况下可能会出现内核清除已注册的事件的事情发生
    throw sheNetException(20,"epoll_wait(get alive fd) system interface error:"+std::string(strerror(errno)));
  } else if (num_events == 0) {
    return {};
  } else {
    std::vector<int> alive_fds;
    for (int i = 0; i <num_events; ++i) {
      // 可读事件,并且该事件不是accept的时候返回
      if (user_events_[i].events == EPOLLIN && (user_events_[i].data.fd != server_socket_fd_)) {
        alive_fds.push_back(user_events_[i].data.fd);
      }
    }
    return alive_fds;
  }
};


