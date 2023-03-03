//
// Created by shecannotsee on 2023/3/2.
//

#include "socket.h"
#include <sheNetException/sheNetException.h>

#include <sys/socket.h>

sheNet::socket::socket()
    : socket(NetTransport::TCP_IPV4) {
};

sheNet::socket::socket(sheNet::NetTransport type) noexcept{
  if (type==NetTransport::TCP_IPV4) {
    id_ = ::socket(AF_INET);
  } else if (type==NetTransport::TCP_IPV6) {

  } else if (type==NetTransport::UDP_IPV4) {

  } else if (type==NetTransport::UDP_IPV6) {

  }
  if (id_ == -1) {
    throw sheNetException(1,"socket create error.");
  }
}
