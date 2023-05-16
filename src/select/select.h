//
// Created by shecannotsee on 2023/3/9.
//

#ifndef SHE_NET_SRC_SELECT_SELECT_H_
#define SHE_NET_SRC_SELECT_SELECT_H_

#include <sys/select.h>

namespace sheNet {

class select {
 public:
  // copy:off
  select(const select&) = delete;
  select& operator=(const select&) = delete;
  // move:off
  select(select&&) = delete;
  select& operator=(select&&) = delete;
  //destructors
  ~select() = default;
  //constructors
  select() = default;
 private:
  // data
 public:
  // interface

};


};// namespace sheNet

#endif //SHE_NET_SRC_SELECT_SELECT_H_
