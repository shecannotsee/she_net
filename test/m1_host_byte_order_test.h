//
// Created by shecannotsee on 2023/3/2.
//

#ifndef SHE_NET_TEST_M1_HOST_BYTE_ORDER_TEST_H_
#define SHE_NET_TEST_M1_HOST_BYTE_ORDER_TEST_H_

#include <iostream>

namespace m1_host_byte_order_test {

int main() {
  union {
    int i;
    char c[sizeof(int)];
  } endianness = {1};

  if (endianness.c[0] == 1) {
    std::cout << "Little endian" << std::endl;
  } else {
    std::cout << "Big endian" << std::endl;
  }

  return 0;
};

};// namespace m1_host_byte_order_test

#endif //SHE_NET_TEST_M1_HOST_BYTE_ORDER_TEST_H_
