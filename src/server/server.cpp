//
// Created by shecannotsee on 2022/10/17.
//

#include "server.h"
#include "base/config.h"
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
      _start(true) {
  std::string section_server = "server";
  std::string ip = "ip";
  std::string port = "port";
  std::string ss,sss,ssss,sssss;
  getConfig(section_server, ip, port, sss, ssss, sssss);
  std::string section_epoll_set = "epoll-set";
  std::string max_event = "max_event";
  getConfig(section_epoll_set,max_event,ss,sss,ssss,sssss);

  _base_socket = DECS::NoCpp14Standard::make_unique<socket_base>(ip,port);
  _epoll_container = DECS::NoCpp14Standard::make_unique<Epoll>(
      _base_socket->getSocketId(),(std::atoi(max_event.c_str())));
};

server::~server() {
  _start = false;
};

void server::start() {
  while (_start) {
    int aliveEvents = _epoll_container->aliveEvents();
    for (int i=0;i<aliveEvents; ++i) {
      auto fd_type = _epoll_container->getEventInfo(i);
      int fd = std::get<0>(fd_type);
      u_int32_t event_type = std::get<1>(fd_type);
      // accept event
      if( fd==_base_socket->getSocketId() && event_type==EPOLLIN ) {
        int client_fd =accept(_base_socket->getSocketId(),NULL,NULL);
        if (client_fd==-1)/* TODO accept error */;
        _epoll_container->addEvent(client_fd);/* TODO:set ET */
        /* debug */ {
          std::cout<<"accept sum : "<<++client_num<<std::endl;
        }
      }
      // can read event
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
          /* now data in buffer,and need to deal */ {
            for (int add_index = 0; add_index < len; ++add_index)
              display += buffer[add_index];
          }
        }
        /* debuf */{
          std::cout<<"get data ["<<display<<std::endl;
        }

      }
      // can write
      else if (event_type==EPOLLOUT) {
        int a=0;
        /* debug */ {
          printf("accept sum : %d\n", --client_num);
        }
      }

    }
  }
}