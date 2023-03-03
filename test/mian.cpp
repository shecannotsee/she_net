//
// Created by shecannotsee on 2023/3/2.
//
#include <iostream>

#include "m1_host_byte_order_test.h"
#include "m2_base_socket_test.h"

int main() {
  std::cout<<"test start.\n";
  m1_host_byte_order_test::main();
  m2_base_socket_test::main();
  std::cout<<"Done.\n";
  return 0;
};
