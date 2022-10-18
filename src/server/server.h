//
// Created by shecannotsee on 2022/10/17.
//

#ifndef DEVICEENROLLMENTCLOUDSERVICE_SRC_SERVER_SERVER_H_
#define DEVICEENROLLMENTCLOUDSERVICE_SRC_SERVER_SERVER_H_

#include "net/socket_base.h"
#include "net/Epoll.h"
#include <string>
#include <tuple>
#include <memory>

namespace DECS {
namespace NoCpp14Standard {
template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts &&... params) {
  return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}
}
}

class server {
  using message = std::tuple<bool,std::string>;
 private:
  message _info;
  std::unique_ptr<socket_base> _base_socket;// 依赖配置的config(需要ip和port)
  std::unique_ptr<Epoll> _epoll_container;// 依赖socket_base
  bool _start;
 public:
  server();
  ~server();
 public:
  void start();
};

#endif //DEVICEENROLLMENTCLOUDSERVICE_SRC_SERVER_SERVER_H_
