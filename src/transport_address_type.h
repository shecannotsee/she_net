//
// Created by shecannotsee on 2023/3/3.
//

#ifndef SHE_NET_SRC_TRANSPORT_ADDRESS_TYPE_H_
#define SHE_NET_SRC_TRANSPORT_ADDRESS_TYPE_H_

namespace she_net {

enum class TRANSPORT_ADDRESS_TYPE : unsigned char {
  TCP_IPV4,
  TCP_IPV6,
  UDP_IPV4,
  UDP_IPV6
};

};// namespace sheNet

#endif //SHE_NET_SRC_TRANSPORT_ADDRESS_TYPE_H_
