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
  // 从内核态拷贝到用户态的事件列表
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
  int aliveEvents(int timeout);
  // call epoll_ctl
  // operation : 添加事件 EPOLL_CTL_ADD,移除事件 EPOLL_CTL_DEL,修改事件 EPOLL_CTL_MOD
  // setEvent : EPOLLIN, EPOLLOUT, EPOLLET
  void modifyEvent(int socket_id,int operation, int setEvent);
  // get event[index],在使用epoll_wait(this->aliveEvents)后使用
  // 返回(活跃)事件的fd以及事件类型( EPOLLIN or EPOLLOUT )
  std::tuple<int, uint32_t> getEventInfo(int index) const;
  message getInfo() const;

};

#endif //DEVICEENROLLMENTCLOUDSERVICE_SRC_NET_EPOLL_H_
