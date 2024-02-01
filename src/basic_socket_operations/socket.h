//
// Created by shecannotsee on 24-2-1.
//

#ifndef SOCKET_H
#define SOCKET_H

#include <netinet/in.h>
#include <sys/socket.h>

#include <cerrno>
#include <stdexcept>

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
  _1 = SOCK_PACKET,
  // Set fd to non-blocking
  NONBLOCK = SOCK_NONBLOCK,
  // When exec is called to create a new process, the fd is automatically closed
  close_when_exec = SOCK_CLOEXEC,
};
enum class protocol : int {
  AUTO_SELECTION = 0,
  // TODO:It needs to be supplemented
  IP  = IPPROTO_IP,
  TCP = IPPROTO_TCP,
  UDP = IPPROTO_UDP,
};
enum class status : int {
  success                                  = 0,
  permission_denied                        = EACCES,
  address_family_not_supported_by_protocol = EAFNOSUPPORT,
  invalid_argument                         = EINVAL,
  too_many_open_files                      = EMFILE,
  no_buffer_space_available                = ENOBUFS,
  out_of_memory                            = ENOMEM,
  protocol_not_supported                   = EPROTONOSUPPORT,
};
}  // namespace socket_param

template <typename T, T V>
struct integral_constant {
  static constexpr T value = V;
  using type               = T;
};

using ipv4   = integral_constant<socket_param::domain, socket_param::domain::IPv4>;
using ipv6   = integral_constant<socket_param::domain, socket_param::domain::IPv6>;
using stream = integral_constant<socket_param::type, socket_param::type::stream>;
using packet = integral_constant<socket_param::type, socket_param::type::datagrams>;
using tcp    = integral_constant<socket_param::protocol, socket_param::protocol::TCP>;
using udp    = integral_constant<socket_param::protocol, socket_param::protocol::UDP>;

template <typename p1, typename p2, typename p3>
struct group_check {
  constexpr group_check() {
    static_assert((p2::value == socket_param::type::stream && p3::value == socket_param::protocol::TCP) ||
                      (p2::value == socket_param::type::datagrams && p3::value == socket_param::protocol::UDP),
                  "Unsupported socket API parameter combinations");
  }
};

class socket_t {
 public:
  template <typename p1, typename p2, typename p3>
  static socket_param::status create() noexcept {
    group_check<p1, p2, p3>();
    const auto status = ::socket(static_cast<int>(p1::value), static_cast<int>(p2::value), static_cast<int>(p3::value));
    if (status > 0) {
      throw int(status);
      return socket_param::status::success;
    } else if (status == 0) {
      throw std::runtime_error("socket return 0,unknown error");
    } else if (status == -1) {
      return static_cast<socket_param::status>(errno);
    }
  }
};

}  // namespace she_net

#endif  // SOCKET_H
