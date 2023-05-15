//
// Created by shecannotsee on 2023/3/2.
//
#include <iostream>

#include "m1_host_byte_order_test.h"
#include "m2_base_socket_test.h"
#include "m3_tcp_io_test.h"
#include "m4_udp_io_test.h"
#include "m5_select_test.h"
#include "m6_poll_test.h"
#include "m7_epoll_test.h"
#include "m8_tcp_bidirectional_io_test.h"
#include "m9_basic_socket_operations.h"

int main() {
  std::cout<<"test start.\n";
//  m1_host_byte_order_test::main();
//  m2_base_socket_test::main();
//  m3_tcp_io_test::main();
//  m4_udp_io_test::main();
//  m5_select_test::main();
//  m6_poll_test::main();
//  m7_epoll_test::main();
//  m8_tcp_bidirectional_io_test::main();
  m9_basic_socket_operations::main();
  std::cout<<"Done.\n";
  return 0;
};
