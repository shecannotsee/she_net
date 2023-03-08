//
// Created by shecannotsee on 2023/3/2.
//

#ifndef SHE_NET_SRC_SOCKET_H_
#define SHE_NET_SRC_SOCKET_H_

#include <string>
#include <net_transport.h>
#include <client_info.h>
#include <quadruple.h>


namespace sheNet {

class socket {
 public:
  socket();
  explicit socket(NetTransport) noexcept;
  ~socket();
  // disable copy
  socket(const socket& x) = delete;
  socket& operator=(const socket& x) = delete;
  // move
  socket(socket&&) = default;
  socket& operator=(socket&& x) = default;
 private:
  NetTransport net_transport_;
  sheNet::quadruple quadruple_;

 public:
  // server api
  void bind(const std::string& port,std::string ip = "0.0.0.0") noexcept;
  void listen(int backlog = 5) noexcept;
  void accept() noexcept;
  // client api
  void connect(const std::string &ip, const std::string &port) noexcept;

 public:
  void            udp_set(sheNet::quadruple quadruple);
  int             get_source_id() const;
  std::string     get_source_ip() const;
  unsigned short  get_source_port() const;
  int             get_destination_id() const;
  std::string     get_destination_ip() const;
  unsigned short  get_destination_port() const;
  NetTransport    get_net_transport() const;

};// class socket

};// namespace sheNet {

#endif //SHE_NET_SRC_SOCKET_H_
