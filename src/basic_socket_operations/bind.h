//
// Created by shecannotsee on 24-2-2.
//

#ifndef BIND_H
#define BIND_H

#include <netinet/in.h>
#include <sys/socket.h>

#include "socket.h"
#include "socket_define.h"

namespace she_net {
namespace bind_param {
// define
template <typename locak_endian, typename protofamily, typename port, port set_port>
struct addr;
// IPv4
template <typename locak_endian, typename port, port set_port>
struct addr<locak_endian, T_V_t<socket_param::domain, socket_param::domain::IPv4>, port, set_port> {
  static constexpr sockaddr_in value{
      .sin_family = static_cast<int>(socket_param::domain::IPv4),
      .sin_port   = hton16(set_port, locak_endian::value),  // set port
      .sin_addr   = {.s_addr = INADDR_ANY}                  // Use INADDR_ANY to represent any address
  };
};
// IPv6
template <typename locak_endian, typename port, port set_port>
struct addr<locak_endian, T_V_t<socket_param::domain, socket_param::domain::IPv6>, port, set_port> {
  static constexpr sockaddr_in6 value{
      .sin6_family = static_cast<int>(socket_param::domain::IPv6),
      .sin6_port   = hton16(set_port, locak_endian::value),  // set port
      .sin6_addr   = IN6ADDR_ANY_INIT                        // Use IN6ADDR_ANY_INIT to represent any address
  };
};

enum class error_info : int {
  permission_denied      = EACCES,
  address_already_in_use = EADDRINUSE,
  socktfd_invalid        = EBADF,
  invalid_argument       = EINVAL,
  non_socket_fd          = ENOTSOCK,
  address_error          = EADDRNOTAVAIL,
  addr_param_error_1     = EFAULT,
  addr_param_error_2     = ELOOP,
  addr_param_is_too_long = ENAMETOOLONG,
  no_file_or_directory   = ENOENT,
  out_of_memory          = ENOMEM,
  not_a_directory        = ENOTDIR,
  read_only_file_system  = EROFS,
};
const std::unordered_map<error_info, std::string> error_info_map = {
    {error_info::permission_denied, "permission_denied"},
    {error_info::address_already_in_use, "address_already_in_use"},
    {error_info::socktfd_invalid, "socktfd_invalid"},
    {error_info::invalid_argument, "invalid_argument"},
    {error_info::non_socket_fd, "non_socket_fd"},
    {error_info::address_error, "address_error"},
    {error_info::addr_param_error_1, "addr_param_error_1"},
    {error_info::addr_param_error_2, "addr_param_error_2"},
    {error_info::addr_param_is_too_long, "addr_param_is_too_long"},
    {error_info::no_file_or_directory, "no_file_or_directory"},
    {error_info::out_of_memory, "out_of_memory"},
    {error_info::not_a_directory, "not_a_directory"},
    {error_info::read_only_file_system, "read_only_file_system"},
};
}  // namespace bind_param

class bind_t {
  static void handle_error(const int status);

 public:
  template <typename locak_endian, typename protofamily, typename port, port set_port>
  static constexpr void exec(socket_fd fd) noexcept {
    constexpr auto addr    = bind_param::addr<locak_endian, protofamily, port, set_port>::value;
    constexpr auto addrlen = sizeof(addr);
    const auto status      = ::bind(fd, (struct sockaddr*)(&addr), addrlen);
    handle_error(status);
  }
};

}  // namespace she_net

#endif  // BIND_H
