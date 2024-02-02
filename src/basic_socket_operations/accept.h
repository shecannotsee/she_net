//
// Created by shecannotsee on 24-2-2.
//

#ifndef ACCEPT_H
#define ACCEPT_H
#include <sys/socket.h>

#include <stdexcept>
#include <unordered_map>

#include "socket.h"
#include "socket_define.h"

namespace she_net {

namespace accept_param {
enum class error_info : int {
  port_already_in_use   = EADDRINUSE,
  socktfd_invalid       = EBADF,
  non_socket_fd         = ENOTSOCK,
  sockets_not_supported = EOPNOTSUPP,
};
const std::unordered_map<error_info, std::string> error_info_map = {
    {error_info::port_already_in_use, "port_already_in_use"},
    {error_info::socktfd_invalid, "socktfd_invalid"},
    {error_info::non_socket_fd, "non_socket_fd"},
    {error_info::sockets_not_supported, "sockets_not_supported"},
};
}  // namespace accept_param

class accept_t {
  static void handle_error(int status);

 public:
  template <typename protofamily>
  static void exec(const socket_fd fd) noexcept {
    auto status = ::accept(fd, nullptr, 0);
    handle_error(status);
  }
};

}  // namespace she_net

#endif  // ACCEPT_H
