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
  // copy
  connection_pool(const connection_pool& x) = delete;
  connection_pool& operator=(const connection_pool& x) = delete;
  // move
  connection_pool(connection_pool&&) = default;
  connection_pool& operator=(connection_pool&& x) = default;
 private:
  std::unordered_multimap<int,bool> connection_pool_;
 public:
  void add(int socket);
  void remove(int socket);


};

};// namespace sheNet

#endif //SHE_NET_SRC_CONNECTION_POOL_H_
