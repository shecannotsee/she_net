//
// Created by shecannotsee on 2023/3/2.
//

#include "socket.h"
#include <sheNetException/sheNetException.h>
#include <make_unique.h>
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
    : quadruple_(),
      net_transport_(type){
  if (net_transport_==NetTransport::TCP_IPV4) {
    quadruple_.source_fd= ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  } else if (net_transport_==NetTransport::TCP_IPV6) {
    quadruple_.source_fd = ::socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
  } else if (net_transport_==NetTransport::UDP_IPV4) {
    quadruple_.source_fd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  } else if (net_transport_==NetTransport::UDP_IPV6) {
    quadruple_.source_fd = ::socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
  }
  if (quadruple_.source_fd == -1) {
    throw sheNetException(1,"socket create error:"+std::string(strerror(errno)));
  }
};

sheNet::socket::~socket() {
  ::shutdown(quadruple_.source_fd,SHUT_RDWR);
};

void sheNet::socket::bind(const std::string& port,std::string ip) noexcept {
  if (ip == "0.0.0.0" && (net_transport_ == NetTransport::TCP_IPV6||
                          net_transport_ == NetTransport::UDP_IPV6  )) {
    ip = "::";
  }
  int ret = -1;
  if (net_transport_==NetTransport::TCP_IPV4) {
    struct sockaddr_in local_address{};
    ::memset(&local_address, 0, sizeof(local_address));
    local_address.sin_family = AF_INET;
    local_address.sin_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
    local_address.sin_addr.s_addr = inet_addr(ip.c_str());
    ret = ::bind(quadruple_.source_fd, (struct sockaddr*)&local_address, sizeof(local_address));
  }
  else if (net_transport_==NetTransport::TCP_IPV6) {
    struct sockaddr_in6 local_address{};
    ::memset(&local_address, 0, sizeof(local_address));
    local_address.sin6_family = AF_INET6;
    local_address.sin6_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
    ::inet_pton(AF_INET6, ip.c_str(), &local_address.sin6_addr);
    ret = ::bind(quadruple_.source_fd, (struct sockaddr*)&local_address, sizeof(local_address));
  }
  else if (net_transport_==NetTransport::UDP_IPV4) {
    struct sockaddr_in local_address{};
    ::memset(&local_address, 0, sizeof(local_address));
    local_address.sin_family = AF_INET;
    local_address.sin_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
    local_address.sin_addr.s_addr = inet_addr(ip.c_str());
    ret = ::bind(quadruple_.source_fd, (struct sockaddr*)&local_address, sizeof(local_address));
  }
  else if (net_transport_==NetTransport::UDP_IPV6) {
    struct sockaddr_in6 local_address{};
    ::memset(&local_address, 0, sizeof(local_address));
    local_address.sin6_family = AF_INET6;
    local_address.sin6_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
    ::inet_pton(AF_INET6, ip.c_str(), &local_address.sin6_addr);
    ret = ::bind(quadruple_.source_fd, (struct sockaddr*)&local_address, sizeof(local_address));
  }

  if (ret == -1) {
    throw sheNetException(2,"bind socket error:"+std::string(strerror(errno)));
  } else {
    quadruple_.source_ip = ip;
    quadruple_.source_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
  }
};

void sheNet::socket::listen(int backlog) noexcept {
  int ret = ::listen(quadruple_.source_fd,backlog);
  if (ret == -1) {
    throw sheNetException(3,"listen port error."+std::string(strerror(errno)));
  }
};

void sheNet::socket::connect(const std::string &ip, const std::string &port) noexcept {
  int connect_results = -1;
  if (net_transport_==NetTransport::TCP_IPV4) {
    struct sockaddr_in server_address{};
    ::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
    ::inet_pton(AF_INET, ip.c_str(), &server_address.sin_addr);
    connect_results = ::connect(quadruple_.source_fd, (struct sockaddr *)&server_address, sizeof(server_address));
  }
  else if (net_transport_==NetTransport::TCP_IPV6) {
    struct sockaddr_in6 server_address{};
    ::memset(&server_address, 0, sizeof(server_address));
    server_address.sin6_family = AF_INET6;
    server_address.sin6_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
    ::inet_pton(AF_INET6, ip.c_str(), &server_address.sin6_addr);
    connect_results = ::connect(quadruple_.source_fd, (struct sockaddr *)&server_address, sizeof(server_address));
  }
  else if (net_transport_==NetTransport::UDP_IPV4) {
    struct sockaddr_in server_address{};
    ::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
    ::inet_pton(AF_INET, ip.c_str(), &server_address.sin_addr);
    connect_results = ::connect(quadruple_.source_fd, (struct sockaddr *)&server_address, sizeof(server_address));
  }
  else if (net_transport_==NetTransport::UDP_IPV6) {
    struct sockaddr_in6 server_address{};
    ::memset(&server_address, 0, sizeof(server_address));
    server_address.sin6_family = AF_INET6;
    server_address.sin6_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
    ::inet_pton(AF_INET6, ip.c_str(), &server_address.sin6_addr);
    connect_results = ::connect(quadruple_.source_fd, (struct sockaddr *)&server_address, sizeof(server_address));
  }

  if (connect_results == -1) {
    throw sheNetException(4,"connect port error."+std::string(strerror(errno)));
  } else {
    quadruple_.destination_ip = ip;
    quadruple_.destination_port = htons(static_cast<unsigned short>(std::atoi(port.c_str())));
  }
};

sheNet::ClientInfo sheNet::socket::accept() noexcept {
  ClientInfo client{};
  int destination_fd = -1;
  if (net_transport_ == NetTransport::TCP_IPV4) {
    struct sockaddr_in client_address{};
    socklen_t client_address_len = sizeof(client_address);
    destination_fd = ::accept(quadruple_.source_fd, (struct sockaddr*)&client_address, &client_address_len);
    //printf("Connection from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
    client.ip_ = std::move(std::string(inet_ntoa(client_address.sin_addr)));
    client.port_ = std::move(std::to_string(ntohs(client_address.sin_port)));
  }
  else if (net_transport_==NetTransport::TCP_IPV6) {
    struct sockaddr_in6 client_address{};
    socklen_t client_address_len = sizeof(client_address);
    destination_fd = ::accept(quadruple_.source_fd, (struct sockaddr*)&client_address, &client_address_len);
    char client_addr_str[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &client_address.sin6_addr, client_addr_str, INET6_ADDRSTRLEN);
    //printf("Connection from %s:%d\n", client_addr_str, ntohs(client_address.sin6_port));
    client.ip_ = std::move(std::string(inet_ntop(AF_INET6, &client_address.sin6_addr, client_addr_str, INET6_ADDRSTRLEN)));
    client.port_ = std::move(std::to_string(ntohs(client_address.sin6_port)));
  }
  if (destination_fd == -1) {
    throw sheNetException(5,"accept port error."+std::string(strerror(errno)));
  } else {
    client.fd_ = CPP11::make_unique<int>(destination_fd);
    quadruple_.destination_fd = destination_fd;
    quadruple_.destination_ip = client.ip_;
    quadruple_.destination_port = std::atoi(client.port_.c_str());
  }
  return client;
};

sheNet::NetTransport sheNet::socket::get_net_transport() const {
  return net_transport_;
};

int sheNet::socket::get_source_id() const {
  return this->quadruple_.source_fd;
};

int sheNet::socket::get_destination_id() const {
  return this->quadruple_.destination_fd;
};
