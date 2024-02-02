//
// Created by shecannotsee on 24-2-1.
//

#ifndef SOCKET_H
#define SOCKET_H

#include <netinet/in.h>
#include <sys/socket.h>

#include <cerrno>
#include <stdexcept>
#include <unordered_map>

#include "socket_define.h"

namespace she_net {
namespace socket_param {
enum class domain : int {
  UNIX_net                      = AF_UNIX,
  local_net                     = AF_LOCAL,
  IPv4                          = AF_INET,
  AX25                          = AF_AX25,
  IPX                           = AF_IPX,
  apple_talk                    = AF_APPLETALK,
  IPv6                          = AF_INET6,
  DEC_net                       = AF_DECnet,
  KEY                           = AF_KEY,
  kernel_user_API_device        = AF_NETLINK,
  low_level_packet_API          = AF_PACKET,
  RDS                           = AF_RDS,
  PPPOX                         = AF_PPPOX,
  LLC                           = AF_LLC,
  InfiniBand                    = AF_IB,
  multiprotocol_label_switching = AF_MPLS,
  CAN                           = AF_CAN,
  TIPC                          = AF_TIPC,
  BLUETOOTH                     = AF_BLUETOOTH,
  kernel_crypto_API             = AF_ALG,
  VSOCK                         = AF_VSOCK,
  kernel_connection_multiplexer = AF_KCM,
  express_data_path             = AF_XDP,
};
enum class type : int {
  // Provides sequenced, reliable, two-way, connection-based byte streams.  An out-of-band data transmission mechanism
  // may be supported.
  stream = SOCK_STREAM,
  // Supports datagrams (connectionless, unreliable messages of a fixed maximum length).
  datagrams = SOCK_DGRAM,
  // Provides a sequenced, reliable, two-way connection-based data transmission path for datagrams of fixed maximum
  // length; a consumer is required to read an entire packet with each input system call.
  seqpacket = SOCK_SEQPACKET,
  // Provides raw network protocol access.
  raw_network = SOCK_RAW,
  // Provides a reliable datagram layer that does not guarantee ordering.
  reliable_datagram_layer = SOCK_RDM,
  // obsolete
  _ = SOCK_PACKET,
  // Set fd to non-blocking
  NONBLOCK = SOCK_NONBLOCK,
  // When exec is called to create a new process, the fd is automatically closed
  close_when_exec = SOCK_CLOEXEC,
};
enum class protocol : int {
  // Usually, this is used
  AUTO_SELECTION = 0,
  // TODO:It needs to be supplemented
  IP   = IPPROTO_IP,
  TCP  = IPPROTO_TCP,
  UDP  = IPPROTO_UDP,
  SCTP = IPPROTO_SCTP,
};
enum class error_info : int {
  permission_denied                        = EACCES,
  address_family_not_supported_by_protocol = EAFNOSUPPORT,
  invalid_argument                         = EINVAL,
  too_many_open_files                      = EMFILE,
  no_buffer_space_available                = ENOBUFS,
  out_of_memory                            = ENOMEM,
  protocol_not_supported                   = EPROTONOSUPPORT,
};
const std::unordered_map<error_info, std::string> error_info_map = {
    {error_info::permission_denied, "permission_denied"},
    {error_info::address_family_not_supported_by_protocol, "address_family_not_supported_by_protocol"},
    {error_info::invalid_argument, "invalid_argument"},
    {error_info::too_many_open_files, "too_many_open_files"},
    {error_info::no_buffer_space_available, "no_buffer_space_available"},
    {error_info::out_of_memory, "out_of_memory"},
    {error_info::protocol_not_supported, "protocol_not_supported"},
};

using ipv4          = T_V_t<domain, domain::IPv4>;
using ipv6          = T_V_t<domain, domain::IPv6>;
using stream        = T_V_t<type, type::stream>;
using packet        = T_V_t<type, type::datagrams>;
using tcp           = T_V_t<protocol, protocol::TCP>;
using udp           = T_V_t<protocol, protocol::UDP>;
using auto_protocol = T_V_t<protocol, protocol::AUTO_SELECTION>;
}  // namespace socket_param

template <typename protofamily, typename type, typename protocol>
struct socket_param_check {
  constexpr socket_param_check() {
    constexpr bool right_cond =
        ((protofamily::value == socket_param::domain::IPv4 || protofamily::value == socket_param::domain::IPv6) &&
         type::value == socket_param::type::stream &&
         (protocol::value == socket_param::protocol::TCP ||
          protocol::value == socket_param::protocol::AUTO_SELECTION)) ||
        ((protofamily::value == socket_param::domain::IPv4 || protofamily::value == socket_param::domain::IPv6) &&
         type::value == socket_param::type::datagrams &&
         (protocol::value == socket_param::protocol::UDP || protocol::value == socket_param::protocol::AUTO_SELECTION));
    static_assert(right_cond, "Unsupported socket API parameter combinations");
  }
};

class socket_t {
  static socket_fd handle_error(const int status);

 public:
  template <typename domain, typename type, typename protocol = socket_param::auto_protocol>
  static socket_fd create() noexcept {
    // compile-time checks
    socket_param_check<domain, type, protocol>();
    // call sys/socket api
    const auto status =
        ::socket(static_cast<int>(domain::value), static_cast<int>(type::value), static_cast<int>(protocol::value));
    // check the results
    return handle_error(status);
  }
};

}  // namespace she_net

#endif  // SOCKET_H
