//
// Created by shecannotsee on 23-5-17.
//

#ifndef SHE_NET_SRC_EPOLL_WRAPPER_EPOLL_WRAPPER_H_
#define SHE_NET_SRC_EPOLL_WRAPPER_EPOLL_WRAPPER_H_

#include <vector>
#include <sys/epoll.h>

namespace sheNet {

class epoll_wrapper {
 public:
  // copy:off
  epoll_wrapper(const epoll_wrapper&) = delete;
  epoll_wrapper& operator=(const epoll_wrapper&) = delete;
  // move:off
  epoll_wrapper(epoll_wrapper&&) = default;
  epoll_wrapper& operator=(epoll_wrapper&&) = default;
  //destructors
  ~epoll_wrapper() = default;
  //constructors
  epoll_wrapper();
  explicit epoll_wrapper(int events_num);

 private:
  int epoll_container_id_;
  std::vector<epoll_event> user_events_;

 public:
  // interface

};

};// namespace sheNet

#endif //SHE_NET_SRC_EPOLL_WRAPPER_EPOLL_WRAPPER_H_
