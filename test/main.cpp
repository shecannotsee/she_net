//
// Created by shecannotsee on 2023/3/2.
//
#include <iostream>

#include "global_test_set.h"
#include "test_example.h"
#include "basic_network_interface_testing/t1_socket_and_io.h"
#include "basic_network_interface_testing/t2_select.h"
#include "basic_network_interface_testing/t3_poll.h"
#include "basic_network_interface_testing/t4_epoll.h"
#include "src_testing/t1_basic_io_operations.h"

#include "m1_host_byte_order_test.h"
#include "m2_base_socket_test.h"
#include "m3_tcp_io_test.h"
#include "m4_udp_io_test.h"
#include "m5_select_test.h"
#include "m6_poll_test.h"
#include "m7_epoll_test.h"
#include "m8_tcp_bidirectional_io_test.h"
#include "m9_basic_socket_operations.h"
#include "m10_basic_io_operations_tcp_test.h"
#include "m11_basic_io_operations_udp_test.h"
#include "m12_select_wrapper_test.h"
#include "m13_poll_wrapper_test.h"
#include "m14_epoll_wrapper_test.h"
#include "m15_stream_data_test.h"

int main() {
  std::cout << YELLOW_COLOR << "Start test " << RESET_COLOR << std::endl;
  constexpr bool easy_test = true;
  constexpr bool test_suite = true;
  if (easy_test) {
    std::cout << GREEN_COLOR << "basic_network_interface_testing::t1_socket_and_io >> main() >> " << YELLOW_COLOR <<
      basic_network_interface_testing::t1_socket_and_io::main() << RESET_COLOR << std::endl;
    std::cout << GREEN_COLOR << "basic_network_interface_testing::t2_select::main() >> " << YELLOW_COLOR <<
      basic_network_interface_testing::t2_select::main() << RESET_COLOR << std::endl;
    std::cout << GREEN_COLOR << "basic_network_interface_testing::t3_poll::main() >> " << YELLOW_COLOR <<
      basic_network_interface_testing::t3_poll::main() << RESET_COLOR << std::endl;
    std::cout << GREEN_COLOR << "basic_network_interface_testing::t4_epoll::main() >> " << YELLOW_COLOR <<
      basic_network_interface_testing::t4_epoll::main() << RESET_COLOR << std::endl;

    //  m1_host_byte_order_test::main();
    //  m2_base_socket_test::main();
    //  m3_tcp_io_test::main();
    //  m4_udp_io_test::main();
    //  m5_select_test::main();
    //  m6_poll_test::main();
    //  m7_epoll_test::main();
    //  m8_tcp_bidirectional_io_test::main();
    //  m9_basic_socket_operations::main();
    //  m10_basic_io_operations_tcp_test::main();
    //  m11_basic_io_operations_udp_test::main();
    //  m12_select_wrapper_test::main();
    //  m13_poll_wrapper_test::main();
    // m14_epoll_wrapper_test::main();
    // m15_stream_data_test::method1::main();
    // m15_stream_data_test::method2::main();
  }

  if (test_suite) {
    testing::InitGoogleTest();

    if (RUN_ALL_TESTS() == 0) {
      std::cout << GREEN_COLOR << "Pass the test." << RESET_COLOR << std::endl;
    } else {
      std::cout << RED_COLOR << "Failed the test." << RESET_COLOR << std::endl;
    }
  }

  return 0;
};
