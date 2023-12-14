//
// Created by shecannotsee on 23-12-14.
//

#ifndef T2_BASIC_IO_OPERATIONS_TCP_H
#define T2_BASIC_IO_OPERATIONS_TCP_H

#include <string>
#include <iostream>
#include <thread>
#include <basic_socket_operations/basic_socket_operations.h>
#include <basic_io_operations/basic_io_operations.h>

namespace src_testing::t2_basic_io_operations_tcp {

std::string main() {
  std::cout << YELLOW_COLOR << "basic_io_operations tcp test start." << RESET_COLOR << std::endl;
  bool server_start = false;

  auto client = std::thread([&](){
    while (server_start == false) {}
    /* connect */
    using BSO/* basic socket operations */
      = she_net::basic_socket_operations;
    int client_fd = BSO::socket();/* set */ {
      BSO::set_socket_noblock(client_fd);
      BSO::port_reuse(client_fd);
    };
    while (true) {
      try {
        BSO::connect(client_fd, "0.0.0.0", "9981");
        // 连接成功则跳出
        break;
      }
      catch (const she_net::she_net_exception& exc) {
        if (exc.get_error_code() == 21) {// 非接口内部问题的异常
          continue;
        } else {// 接口内部的异常
          std::cout << YELLOW_COLOR << exc.what() << RESET_COLOR;// 自带\n
          std::cout << YELLOW_COLOR << "connect failed." << RESET_COLOR << std::endl;
          continue;
        }
      }
    }

    /* io */
    using io = she_net::basic_io_operations::TCP;
    while (true) {
      try {
        static int message_num = 0;
        io::send(client_fd, "No." + std::to_string(++message_num) + " message has been sent.");
        std::cout << YELLOW_COLOR << "No." + std::to_string(message_num) + " message has been sent.\n" << RESET_COLOR;
        if (message_num == 3) {
          break;
        }
      }
      catch (const she_net::she_net_exception& exc) {
        if (exc.get_error_code() != 7) {// 非接口内部问题的异常
          continue;
        } else {// 接口异常
          std::cout << YELLOW_COLOR << "The server is down or unavailable.\n" << RESET_COLOR;
          std::cout << YELLOW_COLOR << "The client will be shut down.\n" << RESET_COLOR;
          break;
        }
      }
    }

    BSO::shutdown(client_fd);
    std::cout << YELLOW_COLOR << "client closed.\n";
  });

  auto server = std::thread([&](){
    /* listen */
    using BSO/* basic socket operations */
        = she_net::basic_socket_operations;
    int server_fd = BSO::socket();/* set */ {
      BSO::port_reuse(server_fd);
      // 非阻塞的accept会抛出大量异常,请谨慎使用
      BSO::set_socket_noblock(server_fd);
      BSO::bind(server_fd,"0.0.0.0","9981");
      BSO::listen(server_fd);
    }

    std::vector<int> client_accept_fds;
    std::mutex mtx;
    bool stop = false;
    auto accept_thread = std::thread([&server_fd,&mtx,&client_accept_fds,&server_start, &stop]() noexcept {
      while (true) {
        try {
          std::unique_lock<std::mutex> lock(mtx);
          server_start = true;
          int client_fd = BSO::accept(server_fd);
          client_accept_fds.push_back(client_fd);
        } catch (const she_net::she_net_exception &exc) {
          if (exc.get_error_code() == 22) {
            sleep(1);
            continue;
          } else {
            std::cout << BLUE_COLOR << exc.what();// 自带\n
            std::cout << BLUE_COLOR << "accept failed.\n" << RESET_COLOR;
            BSO::shutdown(server_fd);
            std::cout << BLUE_COLOR << "server closed.\n" << RESET_COLOR;
            throw exc;
          }
        }
        if (stop) {
          break;
        }
      }
    });

    /* io */
    using io = she_net::basic_io_operations::TCP;
    while (true) {
      try {
        std::unique_lock<std::mutex> lock(mtx);
        static int message_num = 0;
        for (auto client_fd : client_accept_fds) {
          std::string get_message = io::recv(client_fd);
          if (get_message.size() > 0) {
            std::cout << BLUE_COLOR << "[" << get_message << "]\n" << RESET_COLOR;
            static int message_num = 0;
            message_num++;
          }
        }
        if (message_num == 3) {
          break;
        }
      } catch (const she_net::she_net_exception& exc) {
        std::cout << BLUE_COLOR << "recv error" << RESET_COLOR << std::endl;
        sleep(1);
        continue;
      }
    }
    stop = true;
    accept_thread.join();
    BSO::shutdown(server_fd);
    std::cout << BLUE_COLOR << "server closed.\n" << RESET_COLOR;
  });

  client.join();
  server.join();

  return "pass the test";
}

}

#endif //T2_BASIC_IO_OPERATIONS_TCP_H
