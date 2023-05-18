//
// Created by shecannotsee on 23-5-16.
//

#ifndef SHE_NET_TEST_M10_BASIC_IO_OPERATIONS_TEST_H_
#define SHE_NET_TEST_M10_BASIC_IO_OPERATIONS_TEST_H_

#include <iostream>
#include <thread>
#include <basic_socket_operations/basic_socket_operations.h>
#include <basic_io_operations/basic_io_operations.h>

namespace m10_basic_io_operations_tcp_test {

const std::string RESET_COLOR   = "\033[0m";
const std::string RED_COLOR     = "\033[31m";
const std::string GREEN_COLOR   = "\033[32m";
const std::string YELLOW_COLOR  = "\033[33m";

void main() {
  std::cout << RED_COLOR << "basic_io_operations tcp test start." << RESET_COLOR << std::endl;
  sleep(1);

  auto client = std::thread([](){
    /* connect */
    using BSO/* basic socket operations */
      = sheNet::basic_socket_operations;
    int client_fd = BSO::socket();/* set */ {
      BSO::set_socket_noblock(client_fd);
      BSO::port_reuse(client_fd);
    };
    while (true) {
      try {
        BSO::connect(client_fd, "192.168.1.47", "9981");
        break;
      }
      catch (const sheNet::sheNetException& exc) {
        if (exc.get_error_code() == 21) {// 非接口内部问题的异常
          sleep(1);
          continue;
        } else {// 接口内部的异常
          std::cout << YELLOW_COLOR << exc.what() << RESET_COLOR;// 自带\n
          std::cout << YELLOW_COLOR << "connect failed." << RESET_COLOR << std::endl;
          sleep(1);
          continue;
        }
      }
    };

    /* io */
    using io = sheNet::basic_io_operations::TCP;
    while (true) {
      try {
        static int message_num = 0;
        io::send(client_fd, "No." + std::to_string(++message_num) + " message has been sent.");
        std::cout << YELLOW_COLOR << "No." + std::to_string(message_num) + " message has been sent.\n" << RESET_COLOR;
        sleep(1);
      }
      catch (const sheNet::sheNetException& exc) {
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

  auto server = std::thread([](){
    /* listen */
    using BSO/* basic socket operations */
        = sheNet::basic_socket_operations;
    int server_fd = BSO::socket();/* set */ {
      BSO::port_reuse(server_fd);
      // 非阻塞的accept会抛出大量异常,请谨慎使用
      BSO::set_socket_noblock(server_fd);
      BSO::bind(server_fd,"0.0.0.0","9981");
      BSO::listen(server_fd);
    }

    std::vector<int> client_accept_fds;
    std::mutex mtx;
    auto accept_thread = std::thread([&server_fd,&mtx,&client_accept_fds]() noexcept {
      while (true) {
        try {
          std::unique_lock<std::mutex> lock(mtx);
          int client_fd = BSO::accept(server_fd);
          client_accept_fds.push_back(client_fd);
        } catch (const sheNet::sheNetException &exc) {
          if (exc.get_error_code() == 22) {
            sleep(1);
            continue;
          } else {
            std::cout << GREEN_COLOR << exc.what();// 自带\n
            std::cout << GREEN_COLOR << "accept failed.\n" << RESET_COLOR;
            BSO::shutdown(server_fd);
            std::cout << GREEN_COLOR << "server closed.\n" << RESET_COLOR;
            throw exc;
          }
        }
      }
    });

    /* io */
    using io = sheNet::basic_io_operations::TCP;
    while (true) {
      try {
        std::unique_lock<std::mutex> lock(mtx);
        for (auto client_fd : client_accept_fds) {
          std::string get_message = io::recv(client_fd);
          if (get_message.size() > 0) {
            std::cout << GREEN_COLOR << "[" << get_message << "]\n" << RESET_COLOR;
          }
        }
      } catch (const sheNet::sheNetException& exc) {
        std::cout << GREEN_COLOR << "recv error" << RESET_COLOR << std::endl;
        sleep(1);
        continue;
      }
    }

    accept_thread.join();
    BSO::shutdown(server_fd);
    std::cout << GREEN_COLOR << "server closed.\n" << RESET_COLOR;
  });

  client.join();
  server.join();

};

};// namespace m10_basic_io_operations_test

#endif //SHE_NET_TEST_M10_BASIC_IO_OPERATIONS_TEST_H_
