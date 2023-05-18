//
// Created by shecannotsee on 23-5-15.
//

#include "basic_socket_operations.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <cassert>
#include <string.h>
#include <errno.h>
#include <bits/fcntl.h>
#include <fcntl.h>

#include <sheNetException/sheNetException.h>

int sheNet::basic_socket_operations::socket(const sheNet::TRANSPORT_ADDRESS_TYPE type) {
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

void sheNet::basic_socket_operations::listen(int fd, int backlog) {
  int ret = ::listen(fd, backlog);
  if (ret == -1) {
    throw sheNetException(3,"listen port error."+std::string(strerror(errno)));
  }
};

int sheNet::basic_socket_operations::accept(int local_fd, sheNet::TRANSPORT_ADDRESS_TYPE type) {
  int client_fd = -1;
  /*z*/if (type == TRANSPORT_ADDRESS_TYPE::TCP_IPV4) {
    struct sockaddr_in client_address{};
    socklen_t client_address_len = sizeof(client_address);
    client_fd = ::accept(local_fd, (struct sockaddr*)&client_address, &client_address_len);
    /* 在接受连接时获取客户端的ip和端口号 */ {
      std::string ip = std::move(std::string(inet_ntoa(client_address.sin_addr)));
      int port       = ntohs(client_address.sin_port);
      printf("server:client info [%s:%d]\n",ip.c_str(),port);
    };
  }
  else if (type == TRANSPORT_ADDRESS_TYPE::TCP_IPV6) {
    struct sockaddr_in6 client_address{};
    socklen_t client_address_len = sizeof(client_address);
    client_fd = ::accept(local_fd, (struct sockaddr*)&client_address, &client_address_len);
    /* 在接受连接时获取客户端的ip和端口号 */ {
      // TODO:需要实现
    };
  }
  else {
    assert(false);
  }

  if (client_fd == -1) {
    throw sheNetException(5,"accept port error."+std::string(strerror(errno)));
  }

  return client_fd;
};

int sheNet::basic_socket_operations::connect(int local_fd,
                                              std::string ip,
                                              std::string port,
                                              sheNet::TRANSPORT_ADDRESS_TYPE type) {
  int connect_results = -1;
  int local_port = -1;
  /*z*/if (type == TRANSPORT_ADDRESS_TYPE::TCP_IPV4) {
    struct sockaddr_in server_address{};
    ::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
    ::inet_pton(AF_INET, ip.c_str(), &server_address.sin_addr);
    connect_results = ::connect(local_fd, (struct sockaddr *)&server_address, sizeof(server_address));
    /* 在连接时获取本地使用的端口号 */ {
      struct sockaddr_in local_address;
      socklen_t address_length = sizeof(local_address);
      getsockname(local_fd, (struct sockaddr*)&local_address, &address_length);
      // 获取本地连接的端口号
      local_port = ntohs(local_address.sin_port);
    };
  }
  else if (type == TRANSPORT_ADDRESS_TYPE::TCP_IPV6) {
    struct sockaddr_in6 server_address{};
    ::memset(&server_address, 0, sizeof(server_address));
    server_address.sin6_family = AF_INET6;
    server_address.sin6_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
    ::inet_pton(AF_INET6, ip.c_str(), &server_address.sin6_addr);
    connect_results = ::connect(local_fd, (struct sockaddr *)&server_address, sizeof(server_address));
    /* 在连接时获取本地使用的端口号 */ {
      // TODO:需要实现
    };
  }
  else {
    assert(false);
  }

  if (connect_results == -1|| local_port == -1) {
    if (errno == EINPROGRESS || errno == EAGAIN || errno == EWOULDBLOCK) {
      throw sheNetException(21, "connecting..." + std::string(strerror(errno)));
    } else {
      throw sheNetException(4, "connect port error." + std::string(strerror(errno)));
    }
  }

  return local_port;
};

void sheNet::basic_socket_operations::shutdown(int fd) {
  ::shutdown(fd, SHUT_RDWR);
};


void sheNet::basic_socket_operations::port_reuse(int fd, int operations) {
  int reuse = operations ? 1 : 0;
  int ret = ::setsockopt(fd, SOL_SOCKET, SO_REUSEPORT,
                           (const void *)&reuse, sizeof(reuse));
  if (ret == -1) {
    throw sheNetException(10,"set socket opt error."+std::string(strerror(errno)));
  }
};

void sheNet::basic_socket_operations::set_socket_noblock(int fd, bool noblock) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (noblock) {
    flags |= O_NONBLOCK;
  } else {
    flags &= ~O_NONBLOCK;
  }
  int ret = fcntl(fd, F_SETFL, flags);
  if (ret == -1) {
    throw sheNetException(11,"set socket block error."+std::string(strerror(errno)));
  }
};







