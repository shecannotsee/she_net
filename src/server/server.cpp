//
// Created by shecannotsee on 2022/10/17.
//

#include "server.h"
#include "base/config.h"
#include "base/log.h"
#include <string.h>
#include <string>
#include <sys/socket.h>

#include <iostream>
#include <atomic>
std::atomic<unsigned int> client_num;

server::server()
    : _info(std::make_tuple(true,"init")),
      _base_socket(nullptr),
      _epoll_container(nullptr),
      _start(true),
      _logger(nullptr) {
  logModule::registerLogger("server");
  _logger = logModule::getLogger("server");

  std::string section_server = "server";
  std::string ip = "ip";
  std::string port = "port";
  std::string ss,sss,ssss,sssss;
  getConfig(section_server, ip, port, sss, ssss, sssss);
  std::string section_epoll_set = "epoll-set";
  std::string max_event = "max_event";
  getConfig(section_epoll_set,max_event,ss,sss,ssss,sssss);

  _base_socket = DECS::NoCpp14Standard::make_unique<socket_base>(ip,port);
  /* write log */ {
    logModule::registerLogger("socket_base");
    auto logger = logModule::getLogger("socket_base");
    auto socket_base_info = _base_socket->getInfo();
    if (std::get<0>(socket_base_info))
      logger->info(std::get<1>(socket_base_info));
    else
      logger->error(std::get<1>(socket_base_info));
  }
  _epoll_container = DECS::NoCpp14Standard::make_unique<Epoll>(_base_socket->getSocketId(), (std::atoi(max_event.c_str())));
  /* write log */ {
    logModule::registerLogger("epoll");
    auto logger = logModule::getLogger("epoll");
    auto epoll_info = _epoll_container->getInfo();
    if (std::get<0>(epoll_info))
      logger->info(std::get<1>(epoll_info));
    else
      logger->error(std::get<1>(epoll_info));
  }
};

server::~server() {
  _start = false;
};

void server::start() {
  // 添加(实际上就是本地)server fd以便能够触发的accept事件
  _epoll_container->modifyEvent(_base_socket->getSocketId(), EPOLL_CTL_ADD, EPOLLIN);
  while (_start) {
    int aliveEvents = _epoll_container->aliveEvents(-1);
    for (int i=0;i<aliveEvents; ++i) {
      auto fd_type = _epoll_container->getEventInfo(i);
      int fd = std::get<0>(fd_type);
      u_int32_t event_type = std::get<1>(fd_type);

      // event1 : accept event
      if( fd==_base_socket->getSocketId() && event_type==EPOLLIN ) {
        int client_fd =accept(_base_socket->getSocketId(),NULL,NULL);
        if (client_fd==-1)/* TODO : deal accept error */;
        _epoll_container->modifyEvent(client_fd, EPOLL_CTL_ADD,EPOLLIN|EPOLLET);/* ET */
        /* write log */ {
          _logger->info("accept sum : {}",++client_num);
        }
      }
      // event2 :can read event
      else if (event_type == EPOLLIN) {
        std::string display;
        char* buffer[1025];// Magic number
        // -1:read error, 0:client quit, 1:can read
        int readType = 1;
        while (true) {
          memset(buffer, 0x00, sizeof(buffer));
          int len = recv(fd,buffer,1025,NULL);
          if      (len==-1) /* TODO:deal error */break;
          else if (len== 0) /* TODO:deal client quit */break;
          else if (len<1025) break;
          /* now data in buffer,and need to deal */ {
            for (int add_index = 0; add_index < len; ++add_index)
              display += buffer[add_index];
          }
        }
        _logger->info("get data [{}]",display);

      }
      // event3 : can write
      else if (event_type==EPOLLOUT) {
        int a=0;
        _logger->info("accept sum : {}", --client_num);
      }

    }
  }
}