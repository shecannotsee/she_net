//
// Created by shecannotsee on 2023/3/24.
//
// 设计该类的初衷在于管理多个已经建立连接的socket fd
// 并且在map结构中,永远都只有一个fd是open的,也就是connection_pool_[fd_number]为true
// 考虑到在取走fd以后,可能会对fd进行读写,所以同一时间只允许一个fd设置为open,这样能够保证fd能够及时处理
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

 public:// Interface
  int get_open();

 public:
  // io ready
  void set_open(int socket_id);

 public:
  // just allow to accept() to add
  void add(int socket);
  // TODO:need to close socket_fd
  void remove(int socket);

};// class connection_pool

};// namespace sheNet

#endif //SHE_NET_SRC_CONNECTION_POOL_H_
