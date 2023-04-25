//
// Created by shecannotsee on 2023/3/7.
//

#include "message.h"
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sheNetException/sheNetException.h>

sheNet::message::message(std::unique_ptr<sheNet::socket> socket)
    : socket_(socket.release()) {
};

std::string sheNet::message::get() {
  if (socket_->get_net_transport()==TRANSPORT_ADDRESS_TYPE::TCP_IPV4 || socket_->get_net_transport()==TRANSPORT_ADDRESS_TYPE::TCP_IPV6) {
    return this->tcp_get();
  }
  else if (socket_->get_net_transport()==TRANSPORT_ADDRESS_TYPE::UDP_IPV4 || socket_->get_net_transport()==TRANSPORT_ADDRESS_TYPE::UDP_IPV6) {
    return this->udp_get();
  }
};

void sheNet::message::send(const std::string& message) {
  if (socket_->get_net_transport()==TRANSPORT_ADDRESS_TYPE::TCP_IPV4 || socket_->get_net_transport()==TRANSPORT_ADDRESS_TYPE::TCP_IPV6) {
    this->tcp_send(message);
  }
  else if (socket_->get_net_transport()==TRANSPORT_ADDRESS_TYPE::UDP_IPV4 || socket_->get_net_transport()==TRANSPORT_ADDRESS_TYPE::UDP_IPV6) {
    this->udp_send(message);
  }
};

std::string sheNet::message::tcp_get() {
  std::string buffer;
  buffer.resize(1024); // 预留足够的空间
  bool accepting = true;
  while (accepting) {
    // TODO:若超过1024字节,需要重新调整buff的传入指针
    ssize_t number_of_bytes_accepted = recv(socket_->get_destination_id(), &buffer[0], buffer.size(), MSG_DONTWAIT);
    if (number_of_bytes_accepted < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        // 暂时没有数据可读，稍后再试
        accepting = false;
      } else {
        // 出现其他错误，需要处理
        throw sheNetException(6,"tcp recv data error."+std::string(strerror(errno)));
      }
    }
    else {
      // TODO:成功读取了n个字节的数据之后,可能数据仍然不完整,需要做缓冲并且将流数据进行切割处理
      accepting = false;
    }
  };
  return buffer;
};

void sheNet::message::tcp_send(const std::string& message) {
  // 在发送前可能需要将流数据进行头尾包装
  ssize_t n = ::send(socket_->get_source_id(), message.c_str(), message.size(), 0);
  if (n < 0) {
    throw sheNetException(7,"tcp send message error."+std::string(strerror(errno)));
  }
  else if (n<message.size()) {
    // TODO:移动指针继续发送
  }
  else if (n==message.size()) {
    // 成功全部发送
  }

};

std::string sheNet::message::udp_get() {
  std::string buffer;
  buffer.resize(1024);
  bool accepting = true;
  struct sockaddr_in senderAddr;
  socklen_t senderAddrLen = sizeof(senderAddr);
  while (accepting) {
    // TODO:若超过1024字节,需要重新调整buff的传入指针
    ssize_t number_of_bytes_accepted = recvfrom(socket_->get_source_id(), &buffer[0], buffer.size(), NULL,(struct sockaddr*)&senderAddr, &senderAddrLen);
    if (number_of_bytes_accepted < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        // 暂时没有数据可读，稍后再试
        accepting = false;
      }
      else {
        // 出现其他错误，需要处理
        throw sheNetException(8,"upd recv data error."+std::string(strerror(errno)));
      }
    }
    else {
      // TODO:成功读取了n个字节的数据之后,可能数据仍然不完整,需要做缓冲并且将流数据进行切割处理
      accepting = false;
    }
  };
  return buffer;
};

void sheNet::message::udp_send(const std::string& message) {
  int result = -1;
  if (socket_->get_net_transport()==TRANSPORT_ADDRESS_TYPE::UDP_IPV4) {
    struct sockaddr_in server_address{};
    ::memset(&server_address, 0x00, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = ::inet_addr(socket_->get_destination_ip().c_str());
    server_address.sin_port = htons(socket_->get_destination_port());

    result = ::sendto(socket_->get_source_id(), message.c_str(), message.size(), NULL, (struct sockaddr*)&server_address, sizeof(server_address));
  }
  else if (socket_->get_net_transport()==TRANSPORT_ADDRESS_TYPE::UDP_IPV6) {
    struct sockaddr_in6 server_address{};
    ::memset(&server_address, 0x00, sizeof(server_address));
    server_address.sin6_family = AF_INET6;
    ::inet_pton(AF_INET6, socket_->get_destination_ip().c_str(), &server_address.sin6_addr);
    server_address.sin6_port = htons(socket_->get_destination_port());

    result = sendto(socket_->get_source_id(), message.c_str(), message.size(), NULL, (struct sockaddr*)&server_address, sizeof(server_address));
  }

  if (result < 0) {
    throw sheNetException(9,"upd send message error."+std::string(strerror(errno)));
  }
  else if (result<message.size()) {
    // TODO:移动指针继续发送
  }
  else if (result==message.size()) {
    // 成功全部发送
  }

};
