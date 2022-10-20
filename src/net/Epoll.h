//
// Created by shecannotsee on 2022/10/17.
//

#ifndef DEVICEENROLLMENTCLOUDSERVICE_SRC_NET_EPOLL_H_
#define DEVICEENROLLMENTCLOUDSERVICE_SRC_NET_EPOLL_H_

#include <string>
#include <vector>
#include <tuple>
#include <memory>
#include <sys/epoll.h>

class Epoll {
  using message = std::tuple<bool,std::string>;
 private:
  int _epoll_container_id;
  std::vector<epoll_event> _events;
  int _max_events;
  message _info;
 public:
  /* Don't use this function */
  Epoll() = delete;
  explicit Epoll(int socket_id, int max_events);
  ~Epoll();
 public:
  // call epoll_wait
  int aliveEvents();
  // call epoll_ctl
  void addEvent(int socket_id,int operation);
  void removeEvent() = delete;
  void modifyEvent() = delete;
  std::tuple<int, uint32_t> getEventInfo(int index) const;
  message getInfo() const;

};

#endif //DEVICEENROLLMENTCLOUDSERVICE_SRC_NET_EPOLL_H_
