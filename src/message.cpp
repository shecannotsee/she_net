//
// Created by shecannotsee on 2023/3/7.
//

#include "message.h"
#include <sheNetException/sheNetException.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>

sheNet::message::message(std::unique_ptr<sheNet::socket> socket)
    : socket_(socket.release()) {
};

std::string sheNet::message::get() {
  if (socket_->get_net_transport()==NetTransport::TCP_IPV4 || socket_->get_net_transport()==NetTransport::TCP_IPV6) {
    return this->tcp_get();
  }
  else if (socket_->get_net_transport()==NetTransport::TCP_IPV4 || socket_->get_net_transport()==NetTransport::TCP_IPV6) {
    return this->udp_get();
  }
};

void sheNet::message::send(const std::string& message) {
  if (socket_->get_net_transport()==NetTransport::TCP_IPV4 || socket_->get_net_transport()==NetTransport::TCP_IPV6) {
    this->tcp_send(message);
  }
  else if (socket_->get_net_transport()==NetTransport::TCP_IPV4 || socket_->get_net_transport()==NetTransport::TCP_IPV6) {
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
    } else {
      // TODO:成功读取了n个字节的数据之后,可能数据仍然不完整,需要做缓冲并且将流数据进行切割处理
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
//  ssize_t ret = recvfrom(socket_->get_id());
};

void sheNet::message::udp_send(const std::string& message) {

};
