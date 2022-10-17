//
// Created by shecannotsee on 2022/10/17.
//

#ifndef DEVICEENROLLMENTCLOUDSERVICE_SRC_NET_EPOLL_H_
#define DEVICEENROLLMENTCLOUDSERVICE_SRC_NET_EPOLL_H_

#include <string>
#include <tuple>
#include <memory>
#include <sys/epoll.h>

class Epoll {
  using message = std::tuple<bool,std::string>;
 private:
  int _epoll_container_id;
  std::unique_ptr<std::unique_ptr<epoll_event>> _events;
  int _max_events;
 public:
  Epoll();
  explicit Epoll(int);
  ~Epoll();
 public:
  int aliveEvents();
  void addEvent();
  void removeEvent();
  void modifyEvent();

};

#endif //DEVICEENROLLMENTCLOUDSERVICE_SRC_NET_EPOLL_H_
