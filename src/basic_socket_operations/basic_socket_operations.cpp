//
// Created by shecannotsee on 23-5-15.
//

#include "basic_socket_operations.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <cassert>
#include <string.h>
#include <errno.h>

#include <sheNetException/sheNetException.h>

int sheNet::basic_socket_operations::socket(const sheNet::TRANSPORT_ADDRESS_TYPE type = TRANSPORT_ADDRESS_TYPE::TCP_IPV4) {
  int ret_fd = -1;
  /*zzz*/if (type==TRANSPORT_ADDRESS_TYPE::TCP_IPV4) {
    ret_fd= ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  } else if (type==TRANSPORT_ADDRESS_TYPE::TCP_IPV6) {
    ret_fd = ::socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
  } else if (type==TRANSPORT_ADDRESS_TYPE::UDP_IPV4) {
    ret_fd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  } else if (type==TRANSPORT_ADDRESS_TYPE::UDP_IPV6) {
    ret_fd = ::socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
  } else {
    assert(false);
  }

  if (ret_fd == -1) {
    throw sheNetException(1,"socket create error:"+std::string(strerror(errno)));
  }

  return ret_fd;
};




