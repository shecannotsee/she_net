//
// Created by shecannotsee on 24-2-1.
//

#include "socket.h"
she_net::socket_fd she_net::socket_t::handle_error(const int status) {
  if (status > 0) {
    // success
    return status;
  } else if (status == -1) {
    const auto error_info = static_cast<socket_param::error_info>(errno);
    if (const auto it = socket_param::error_info_map.find(error_info); it != socket_param::error_info_map.end()) {
      const auto error_message = it->second;
      // error handling
      throw std::runtime_error("Socket creation failed: " + error_message);
    } else {
      // Handle the case where error_info is not found in the map
      throw std::runtime_error("Socket creation failed with unknown error");
    }
  } else {
    throw std::runtime_error("Error: socket returned " + std::to_string(status));
  }
}
