//
// Created by shecannotsee on 2023/3/24.
//

#ifndef SHE_NET_SRC_CONNECTION_POOL_H_
#define SHE_NET_SRC_CONNECTION_POOL_H_

#include <unordered_map>

namespace sheNet {

class connection_pool {
 public:
  connection_pool();
  ~connection_pool();
  // disable
  connection_pool(const connection_pool& x) = delete;
  connection_pool& operator=(const connection_pool& x) = delete;
  // move
  connection_pool(connection_pool&&) = default;
  connection_pool& operator=(connection_pool&& x) = default;

 private:
  std::unordered_multimap<int,bool> connection_pool_;
  int open_socket_fd_;

 public:
  // io ready
  void set_open(int socket_id);

 public:
  // just allow to accept() to add
  void add(int socket);
  void remove(int socket);

};// class connection_pool

};// namespace sheNet

#endif //SHE_NET_SRC_CONNECTION_POOL_H_
