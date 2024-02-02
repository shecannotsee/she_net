//
// Created by shecannotsee on 24-2-2.
//

#ifndef SOCKET_DEFINE_H
#define SOCKET_DEFINE_H

namespace she_net {

template <typename T, T V>
struct T_V_t {
  static constexpr T value = V;
  using type               = T;
};

using socket_fd = int32_t;
using port      = u_int16_t;

enum class endian {
  little,
  big,
};

using local_b = T_V_t<endian, endian::big>;
using local_l = T_V_t<endian, endian::little>;

// TODO:Use template to replace it
#define ENDIAN_SWAP16(A) ((((uint16_t)(A) & 0xff00) >> 8) | (((uint16_t)(A) & 0x00ff) << 8))

#define ENDIAN_SWAP32(A)                                                                                              \
  ((((uint32_t)(A) & 0xff000000) >> 24) | (((uint32_t)(A) & 0x00ff0000) >> 8) | (((uint32_t)(A) & 0x0000ff00) << 8) | \
   (((uint32_t)(A) & 0x000000ff) << 24))
/*
 * htonl/ntohl/htons/ntohs impl
 */
#define hton32(h, e) (e == endian::big) ? h : ENDIAN_SWAP32(h)
#define ntoh32(n, e) (e == endian::big) ? n : ENDIAN_SWAP32(n)
#define hton16(h, e) (e == endian::big) ? h : ENDIAN_SWAP16(h)
#define ntoh16(n, e) (e == endian::big) ? n : ENDIAN_SWAP16(n)

// The network is always the big endian
#define HOST_TO_NETWORK_32(net) htonl(net)
#define NETWORK_TO_HOST_32(host) ntohl(host)
#define HOST_TO_NETWORK_16(net) htons(net)
#define NETWORK_TO_HOST_16(host) ntohs(host)

// replace end

}  // namespace she_net

#endif  // SOCKET_DEFINE_H
