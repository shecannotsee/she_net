//
// Created by shecannotsee on 23-5-17.
//

#ifndef SHE_NET_SRC_POLL_WRAPPER_POLL_WRAPPER_H_
#define SHE_NET_SRC_POLL_WRAPPER_POLL_WRAPPER_H_

namespace sheNet {

class poll_wrapper {
 public:
  // copy:off
  poll_wrapper(const poll_wrapper&) = delete;
  poll_wrapper& operator=(const poll_wrapper&) = delete;
  // move:off
  poll_wrapper(poll_wrapper&&) = delete;
  poll_wrapper& operator=(poll_wrapper&&) = delete;
  //destructors
  ~poll_wrapper() = default;
  //constructors
  poll_wrapper() = default;

 private:
  // data
 public:
  // interface

};

};// namespace sheNet

#endif //SHE_NET_SRC_POLL_WRAPPER_POLL_WRAPPER_H_
