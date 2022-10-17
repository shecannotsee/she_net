//
// Created by shecannotsee on 2022/10/17.
//

#ifndef DEVICEENROLLMENTCLOUDSERVICE_SRC_SERVER_SERVER_H_
#define DEVICEENROLLMENTCLOUDSERVICE_SRC_SERVER_SERVER_H_

#include "net/socket_base.h"
#include "net/Epoll.h"
#include <string>
#include <tuple>

class server {
  using message = std::tuple<bool,std::string>;
 private:
  socket_base _base_socket;
  Epoll _epoll_container;
};

#endif //DEVICEENROLLMENTCLOUDSERVICE_SRC_SERVER_SERVER_H_
