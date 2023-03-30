//
// Created by shecannotsee on 2023/3/24.
//

#include "connection_pool.h"

void sheNet::connection_pool::set_open(int socket_id) {
  connection_pool_[socket_id] = true;
};

void sheNet::connection_pool::add(int socket_id) {
  connection_pool_.insert(std::make_pair(socket_id, false));
};

void sheNet::connection_pool::remove(int socket_id) {
  connection_pool_.erase(socket_id);
};

