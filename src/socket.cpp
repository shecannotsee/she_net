//
// Created by shecannotsee on 2023/3/2.
//

#include "socket.h"
#include <sheNetException/sheNetException.h>
#include <errno.h>

#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

sheNet::socket::socket()
    : socket(NetTransport::TCP_IPV4) {
};

sheNet::socket::socket(sheNet::NetTransport type) noexcept
    : id_(-1),
      net_transport_(type){
  if (net_transport_==NetTransport::TCP_IPV4) {
    id_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  } else if (net_transport_==NetTransport::TCP_IPV6) {
    id_ = ::socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
  } else if (net_transport_==NetTransport::UDP_IPV4) {
    id_ = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  } else if (net_transport_==NetTransport::UDP_IPV6) {
    id_ = ::socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
  }
  if (id_ == -1) {
    throw sheNetException(1,"socket create error:"+std::string(strerror(errno)));
  }
};

sheNet::socket::~socket() {
  ::shutdown(id_,SHUT_RDWR);
};

void sheNet::socket::bind(const std::string& ip,const std::string& port) noexcept {
  int ret = -1;
  if (net_transport_==NetTransport::TCP_IPV4) {
    struct sockaddr_in local_address{};
    ::memset(&local_address, 0, sizeof(local_address));
    local_address.sin_family = AF_INET;
    local_address.sin_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
    local_address.sin_addr.s_addr = inet_addr(ip.c_str());
    ret = ::bind(id_, (struct sockaddr*)&local_address, sizeof(local_address));
  }
  else if (net_transport_==NetTransport::TCP_IPV6) {
    struct sockaddr_in6 local_address{};
    ::memset(&local_address, 0, sizeof(local_address));
    local_address.sin6_family = AF_INET6;
    local_address.sin6_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
    ::inet_pton(AF_INET6, ip.c_str(), &local_address.sin6_addr);
    ret = ::bind(id_, (struct sockaddr*)&local_address, sizeof(local_address));
  }
  else if (net_transport_==NetTransport::UDP_IPV4) {
    struct sockaddr_in local_address{};
    ::memset(&local_address, 0, sizeof(local_address));
    local_address.sin_family = AF_INET;
    local_address.sin_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
    local_address.sin_addr.s_addr = inet_addr(ip.c_str());
    ret = ::bind(id_, (struct sockaddr*)&local_address, sizeof(local_address));
  }
  else if (net_transport_==NetTransport::UDP_IPV6) {
    struct sockaddr_in6 local_address{};
    ::memset(&local_address, 0, sizeof(local_address));
    local_address.sin6_family = AF_INET6;
    local_address.sin6_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
    ::inet_pton(AF_INET6, ip.c_str(), &local_address.sin6_addr);
    ret = ::bind(id_, (struct sockaddr*)&local_address, sizeof(local_address));
  }

  if (ret == -1) {
    throw sheNetException(2,"bind socket error:"+std::string(strerror(errno)));
  }
}
void sheNet::socket::listen(int backlog) noexcept {
  int ret = ::listen(id_,backlog);
  if (ret== -1) {
    throw sheNetException(3,"listen port error."+std::string(strerror(errno)));
  }
};

void sheNet::socket::connect(const std::string &ip, const std::string &port) noexcept {
  int ret = -1;
  if (net_transport_==NetTransport::TCP_IPV4) {
    struct sockaddr_in server_address{};
    ::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
    ::inet_pton(AF_INET, ip.c_str(), &server_address.sin_addr);
    ret = ::connect(id_, (struct sockaddr *)&server_address, sizeof(server_address));
  }
  else if (net_transport_==NetTransport::TCP_IPV6) {
    struct sockaddr_in6 server_address{};
    ::memset(&server_address, 0, sizeof(server_address));
    server_address.sin6_family = AF_INET6;
    server_address.sin6_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
    ::inet_pton(AF_INET6, ip.c_str(), &server_address.sin6_addr);
    ret = ::connect(id_, (struct sockaddr *)&server_address, sizeof(server_address));
  }
  else if (net_transport_==NetTransport::UDP_IPV4) {
    struct sockaddr_in server_address;
    ::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
    ::inet_pton(AF_INET, ip.c_str(), &server_address.sin_addr);
    ret = ::connect(id_, (struct sockaddr *)&server_address, sizeof(server_address));
  }
  else if (net_transport_==NetTransport::UDP_IPV6) {
    struct sockaddr_in6 server_address{};
    ::memset(&server_address, 0, sizeof(server_address));
    server_address.sin6_family = AF_INET6;
    server_address.sin6_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
    ::inet_pton(AF_INET6, ip.c_str(), &server_address.sin6_addr);
    ret = ::connect(id_, (struct sockaddr *)&server_address, sizeof(server_address));
  }

  if (ret == -1) {
    throw sheNetException(4,"connect port error."+std::string(strerror(errno)));
  }
};
