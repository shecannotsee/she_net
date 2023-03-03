//
// Created by shecannotsee on 2023/3/3.
//

#ifndef SHE_NET_SRC_NET_TRANSPORT_H_
#define SHE_NET_SRC_NET_TRANSPORT_H_

namespace sheNet {

enum class NetTransport : unsigned char {
  TCP_IPV4,
  TCP_IPV6,
  UDP_IPV4,
  UDP_IPV6
};

};// namespace sheNet

#endif //SHE_NET_SRC_NET_TRANSPORT_H_
