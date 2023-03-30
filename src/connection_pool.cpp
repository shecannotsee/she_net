//
// Created by shecannotsee on 2023/3/24.
//

#include "connection_pool.h"

int sheNet::connection_pool::get_open() {
  connection_pool_[open_socket_fd_] = false;
  return open_socket_fd_;
};


void sheNet::connection_pool::set_open(int socket_id) {
  connection_pool_[socket_id] = true;
};

void sheNet::connection_pool::add(int socket_id) {
  connection_pool_.insert(std::make_pair(socket_id, false));
};

void sheNet::connection_pool::remove(int socket_id) {
  connection_pool_.erase(socket_id);
};

