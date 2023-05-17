//
// Created by shecannotsee on 23-5-17.
//

#ifndef SHE_NET_SRC_EPOLL_WRAPPER_EPOLL_WRAPPER_H_
#define SHE_NET_SRC_EPOLL_WRAPPER_EPOLL_WRAPPER_H_

namespace sheNet {

class epoll_wrapper {
 public:
  // copy:off
  epoll_wrapper(const epoll_wrapper&) = delete;
  epoll_wrapper& operator=(const epoll_wrapper&) = delete;
  // move:off
  epoll_wrapper(epoll_wrapper&&) = delete;
  epoll_wrapper& operator=(epoll_wrapper&&) = delete;
  //destructors
  ~epoll_wrapper() = default;
  //constructors
  epoll_wrapper() = default;

 private:
  // data
 public:
  // interface

};

};// namespace sheNet

#endif //SHE_NET_SRC_EPOLL_WRAPPER_EPOLL_WRAPPER_H_
