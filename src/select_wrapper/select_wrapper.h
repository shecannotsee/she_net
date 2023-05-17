//
// Created by shecannotsee on 2023/3/9.
//

#ifndef SHE_NET_SRC_SELECT_SELECT_WRAPPER_H_
#define SHE_NET_SRC_SELECT_SELECT_WRAPPER_H_

namespace sheNet {

class select_wrapper {
 public:
  // copy:off
  select_wrapper(const select_wrapper&) = delete;
  select_wrapper& operator=(const select_wrapper&) = delete;
  // move:off
  select_wrapper(select_wrapper&&) = delete;
  select_wrapper& operator=(select_wrapper&&) = delete;
  //destructors
  ~select_wrapper() = default;
  //constructors
  select_wrapper() = default;
 private:
  // data
 public:
  // interface

};



};// namespace sheNet

#endif //SHE_NET_SRC_SELECT_SELECT_WRAPPER_H_
