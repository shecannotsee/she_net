//
// Created by shecannotsee on 2023/3/3.
//

#ifndef SHE_NET_SRC_CLIENT_INFO_H_
#define SHE_NET_SRC_CLIENT_INFO_H_

#include <string>
#include <memory>

namespace sheNet {

class ClientInfo {
 public:
  std::string ip_;
  std::string port_;
  std::unique_ptr<int> fd_;

};// class ClientInfo

};// namespace sheNet

#endif //SHE_NET_SRC_CLIENT_INFO_H_
