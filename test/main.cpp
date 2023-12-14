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
#include "src_testing/t1_basic_socket_operations.h"
#include "src_testing/t2_basic_io_operations_tcp.h"
#include "src_testing/t3_basic_io_operations_udp.h"
#include "src_testing/t4_select_wrapper.h"
#include "src_testing/t5_poll_wrapper.h"
#include "src_testing/t6_epoll_wrapper.h"
#include "src_testing/t7_stream_data.h"

#include "m1_host_byte_order_test.h"
#include "m2_base_socket_test.h"
#include "m3_tcp_io_test.h"
#include "m4_udp_io_test.h"
#include "m5_select_test.h"
#include "m6_poll_test.h"
#include "m7_epoll_test.h"
#include "m8_tcp_bidirectional_io_test.h"


int main() {
  std::cout << YELLOW_COLOR << "Start test " << RESET_COLOR << std::endl;
  constexpr bool easy_test = true;
  constexpr bool test_suite = true;
  if (easy_test) {
    /* basic_network_interface_testing */
    // t1
    std::cout << GREEN_COLOR << "basic_network_interface_testing::t1_socket_and_io >> main() >> " << YELLOW_COLOR <<
      basic_network_interface_testing::t1_socket_and_io::main() << RESET_COLOR << std::endl;
    // t2
    std::cout << GREEN_COLOR << "basic_network_interface_testing::t2_select::main() >> " << YELLOW_COLOR <<
      basic_network_interface_testing::t2_select::main() << RESET_COLOR << std::endl;
    // t3
    std::cout << GREEN_COLOR << "basic_network_interface_testing::t3_poll::main() >> " << YELLOW_COLOR <<
      basic_network_interface_testing::t3_poll::main() << RESET_COLOR << std::endl;
    // t4
    std::cout << GREEN_COLOR << "basic_network_interface_testing::t4_epoll::main() >> " << YELLOW_COLOR <<
      basic_network_interface_testing::t4_epoll::main() << RESET_COLOR << std::endl;

    /* src_testing */
    // t1
    std::cout << GREEN_COLOR << "src_testing::t1_basic_socket_operations::main() >> " << YELLOW_COLOR <<
      src_testing::t1_basic_socket_operations::main() << RESET_COLOR << std::endl;
    // t2
    std::cout << GREEN_COLOR << "src_testing::t2_basic_io_operations_tcp::main() >> " << YELLOW_COLOR <<
      src_testing::t2_basic_io_operations_tcp::main() << RESET_COLOR << std::endl;
    // t3
    std::cout << GREEN_COLOR << "src_testing::t3_basic_io_operations_udp::main() >> " << YELLOW_COLOR <<
      src_testing::t3_basic_io_operations_udp::main() << RESET_COLOR << std::endl;
    // t4
    std::cout << GREEN_COLOR << "src_testing::t4_select_wrapper::main() >> " << YELLOW_COLOR <<
      src_testing::t4_select_wrapper::main() << RESET_COLOR << std::endl;
    // t5
    std::cout << GREEN_COLOR << "src_testing::t5_poll_wrapper::main() >> " << YELLOW_COLOR <<
      src_testing::t5_poll_wrapper::main() << RESET_COLOR << std::endl;
    // t6
    std::cout << GREEN_COLOR << "src_testing::t6_epoll_wrapper::main() >> " << YELLOW_COLOR <<
      src_testing::t6_epoll_wrapper::main() << RESET_COLOR << std::endl;
    // t7
    std::cout << GREEN_COLOR << "src_testing::t7_stream_data::main() >> " << YELLOW_COLOR <<
      src_testing::t7_stream_data::main() << RESET_COLOR << std::endl;

    //  m1_host_byte_order_test::main();
    //  m2_base_socket_test::main();
    //  m3_tcp_io_test::main();
    //  m4_udp_io_test::main();
    //  m5_select_test::main();
    //  m6_poll_test::main();
    //  m7_epoll_test::main();
    //  m8_tcp_bidirectional_io_test::main();
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
