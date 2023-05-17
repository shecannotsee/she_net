//
// Created by shecannotsee on 23-5-17.
//

#include "epoll_wrapper.h"

#include <sys/epoll.h>

#include <make_unique.h>

sheNet::epoll_wrapper::epoll_wrapper()
    : epoll_wrapper(1024) {};

sheNet::epoll_wrapper::epoll_wrapper(int events_num)
    : user_events_(events_num) {
  epoll_container_id_ = epoll_create(1);
};
