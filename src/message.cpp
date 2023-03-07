//
// Created by shecannotsee on 2023/3/7.
//

#include "message.h"
#include <sheNetException/sheNetException.h>

sheNet::message::message(std::unique_ptr<sheNet::socket> socket)
    : socket_(socket.release()) {
};

std::string sheNet::message::get() {
  if (socket_->get_net_transport()==NetTransport::TCP_IPV4 || socket_->get_net_transport()==NetTransport::TCP_IPV6) {
    this->tcp_get();
  }
  else if (socket_->get_net_transport()==NetTransport::TCP_IPV4 || socket_->get_net_transport()==NetTransport::TCP_IPV6) {
    this->udp_get();
  }
};

void sheNet::message::send(std::string&& str) {
  if (socket_->get_net_transport()==NetTransport::TCP_IPV4 || socket_->get_net_transport()==NetTransport::TCP_IPV6) {
    this->tcp_send();
  }
  else if (socket_->get_net_transport()==NetTransport::TCP_IPV4 || socket_->get_net_transport()==NetTransport::TCP_IPV6) {
    this->udp_send();
  }
};

void sheNet::message::tcp_get() {

};

void sheNet::message::tcp_send() {

};

void sheNet::message::udp_get() {

};

void sheNet::message::udp_send() {

};
