//
// Created by shecannotsee on 2023/3/30.
//

#include <connection_pool.h>

#ifndef SHE_NET_SRC_MESSAGE_WITH_CONNECTION_POOL_H_
#define SHE_NET_SRC_MESSAGE_WITH_CONNECTION_POOL_H_

class message_with_connection_pool {
 public:
  message_with_connection_pool() = delete;
  message_with_connection_pool(std::unique_ptr<sheNet::socket> socket);
  ~message_with_connection_pool() = default;
  // disable copy
  message_with_connection_pool(const message_with_connection_pool& x) = delete;
  message_with_connection_pool& operator=(const message_with_connection_pool& x) = delete;
  // open move
  message_with_connection_pool(message_with_connection_pool&&) = default;
  message_with_connection_pool& operator=(message_with_connection_pool&& x) = default;
 private:
  connection_pool socket_pool_;

 public:
  std::string   get();
  void          send(const std::string& message);

 private:
  std::string   tcp_get();
  void          tcp_send(const std::string& message);
  std::string   udp_get();
  void          udp_send(const std::string& message);


};

#endif //SHE_NET_SRC_MESSAGE_WITH_CONNECTION_POOL_H_
