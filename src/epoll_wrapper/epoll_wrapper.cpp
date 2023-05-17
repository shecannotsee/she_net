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
  epoll_container_id_ = epoll_create(1);
};

void sheNet::epoll_wrapper::add_server_fd(int local_fd) {
  server_socket_fd_ = local_fd;
};

void sheNet::epoll_wrapper::add_alive_fd(int fd ,EPOLL_EVENTS trigger_mode) const {
  epoll_event event_options{};
  event_options.events  = EPOLLIN | trigger_mode;
  event_options.data.fd = fd;
  int control_result = epoll_ctl(epoll_container_id_,EPOLL_CTL_ADD,server_socket_fd_,&event_options);
  if (control_result == -1) {
    throw sheNetException(17,"epoll_ctl add system interface error:"+std::string(strerror(errno)));
  }
};
