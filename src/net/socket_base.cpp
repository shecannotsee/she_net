//
// Created by shecannotsee on 2022/10/17.
//
#include "socket_base.h"
#include <string>
#include <sys/socket.h>//socket,bind,listen
#include <arpa/inet.h>//htons,inet_addr,(struct)sockaddr_in
#include <unistd.h>//close

socket_base::socket_base()
    : socket_base("127.0.0.1","26758") {
};

socket_base::socket_base(std::string ip,std::string port)
    : _type("server"),
      _socket_id(-1),
      _info(std::make_tuple(true,"init.")) {
  sockaddr_in serverAddr; /* init */ {
    serverAddr.sin_family = PF_INET;//ipv4
    serverAddr.sin_port = htons(std::atoi(port.c_str()));
    serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
  }
  _socket_id = socket(PF_INET, SOCK_STREAM, 0);
  /* set no TIME_WAIT */ {
    int optval = 1;
    setsockopt(_socket_id, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  }
  int bind_result = bind(_socket_id, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
  if (bind_result==-1)  _info = std::make_tuple(false,"bind error.");
  int listen_result = listen(_socket_id, 5);
  if (listen_result==-1) _info = std::make_tuple(false,"listen error");
  //fcntl(server_socket_id, F_SETFL, O_NONBLOCK); // set noblock
};

socket_base::~socket_base() {
  close(_socket_id);
};

std::string socket_base::getSocketType() const {
  return _type;
};

int socket_base::getSocketId() const {
  return _socket_id;
};

socket_base::message socket_base::getInfo() const {
  return _info;
}
