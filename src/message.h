//
// Created by shecannotsee on 2023/3/7.
//

#ifndef SHE_NET_SRC_MESSAGE_H_
#define SHE_NET_SRC_MESSAGE_H_

#include "socket.h"

namespace sheNet {

class message {
 public:
  message() = delete;
  message(int);
  ~message() = default;
  // copy
  message(const message& x) = default;
  message& operator=(const message& x) = default;
  // move
  message(message&&) = default;
  message& operator=(message&& x) = default;
 private:

 public:



};

};// namespace sheNet

#endif //SHE_NET_SRC_MESSAGE_H_
