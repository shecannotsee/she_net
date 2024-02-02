//
// Created by shecannotsee on 24-2-2.
//

#include "listen.h"

namespace she_net {
void listen_t::handle_error(const int status) {
  if (status == 0) {
    // success
    return;
  } else if (status == -1) {
    const auto error_info = static_cast<listen_param::error_info>(errno);
    if (const auto it = listen_param::error_info_map.find(error_info); it != listen_param::error_info_map.end()) {
      const auto error_message = it->second;
      // error handling
      throw std::runtime_error("bind socket failed: " + error_message);
    } else {
      // Handle the case where error_info is not found in the map
      throw std::runtime_error("bind socket failed with unknown error");
    }
  } else {
    throw std::runtime_error("Error: bind socket returned " + std::to_string(status));
  }
}
}  // namespace she_net
