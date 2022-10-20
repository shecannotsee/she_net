//
// Created by shecannotsee on 2022/10/17.
//

#include "Epoll.h"
#include <unistd.h>//close

Epoll::Epoll(int socket_id, int max_events)
    : _info(std::make_tuple(true,"init success.")),
      _max_events(max_events),
      _events(std::vector<epoll_event>(max_events)),
      _epoll_container_id(-1) {
  if (socket_id==-1) _info = std::make_tuple(false,"init error.");
  _epoll_container_id = epoll_create(1);
};

Epoll::~Epoll() {
  close(_epoll_container_id);
};

int Epoll::aliveEvents() {
  return epoll_wait(_epoll_container_id,
                    &*_events.begin(),
                    _events.size(),
                    -1);
}

void Epoll::addEvent(int socket_id,int operation) {
  epoll_event ev;
  ev.data.fd = socket_id;
  ev.events = operation;
  epoll_ctl(_epoll_container_id, EPOLL_CTL_ADD, socket_id, &ev);
};

std::tuple<int, uint32_t> Epoll::getEventInfo(int index) const {
  return std::make_tuple(_events[index].data.fd, _events[index].events);
};


Epoll::message Epoll::getInfo() const {
  return _info;
};
