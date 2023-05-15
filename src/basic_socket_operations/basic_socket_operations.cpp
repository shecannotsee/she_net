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
}
void sheNet::basic_socket_operations::bind(int fd,
                                           std::string ip,
                                           std::string port,
                                           sheNet::TRANSPORT_ADDRESS_TYPE type) {
  // 若传入的是ipv6的"0.0.0.0"也就是所有地址,默认转换成IPV6的全地址
  if (ip == "0.0.0.0" &&
        (type == TRANSPORT_ADDRESS_TYPE::TCP_IPV6||
        type == TRANSPORT_ADDRESS_TYPE::UDP_IPV6  )) {
    ip = "::";
  };
  int operation_results = -1;
  /***/if (type==TRANSPORT_ADDRESS_TYPE::TCP_IPV4) {
    struct sockaddr_in local_address{};
    ::memset(&local_address, 0, sizeof(local_address));
    local_address.sin_family = AF_INET;
    local_address.sin_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
    local_address.sin_addr.s_addr = ::inet_addr(ip.c_str());
    operation_results = ::bind(fd, (struct sockaddr*)&local_address, sizeof(local_address));
  }
  else if (type==TRANSPORT_ADDRESS_TYPE::TCP_IPV6) {
    struct sockaddr_in6 local_address{};
    ::memset(&local_address, 0, sizeof(local_address));
    local_address.sin6_family = AF_INET6;
    local_address.sin6_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
    ::inet_pton(AF_INET6, ip.c_str(), &local_address.sin6_addr);
    operation_results = ::bind(fd, (struct sockaddr*)&local_address, sizeof(local_address));
  }
  else if (type==TRANSPORT_ADDRESS_TYPE::UDP_IPV4) {
    struct sockaddr_in local_address{};
    ::memset(&local_address, 0, sizeof(local_address));
    local_address.sin_family = AF_INET;
    local_address.sin_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
    local_address.sin_addr.s_addr = ::inet_addr(ip.c_str());
    operation_results = ::bind(fd, (struct sockaddr*)&local_address, sizeof(local_address));
  }
  else if (type==TRANSPORT_ADDRESS_TYPE::UDP_IPV6) {
    struct sockaddr_in6 local_address{};
    ::memset(&local_address, 0, sizeof(local_address));
    local_address.sin6_family = AF_INET6;
    local_address.sin6_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
    ::inet_pton(AF_INET6, ip.c_str(), &local_address.sin6_addr);
    operation_results = ::bind(fd, (struct sockaddr*)&local_address, sizeof(local_address));
  }
  else {
    assert(false);
  }

  if (operation_results == -1) {
    throw sheNetException(2,"bind socket error:"+std::string(strerror(errno)));
  }
};






