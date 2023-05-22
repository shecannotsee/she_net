//
// Created by shecannotsee on 23-5-17.
//

#ifndef SHE_NET_TEST_M14_EPOLL_WRAPPER_TEST_H_
#define SHE_NET_TEST_M14_EPOLL_WRAPPER_TEST_H_

#include <string>
#include <future>
#include <basic_socket_operations/basic_socket_operations.h>
#include <basic_io_operations/basic_io_operations.h>
#include <epoll_wrapper/epoll_wrapper.h>

namespace m14_epoll_wrapper_test {

const std::string RESET_COLOR   = "\033[0m";
const std::string RED_COLOR     = "\033[31m";
const std::string GREEN_COLOR   = "\033[32m";
const std::string YELLOW_COLOR  = "\033[33m";

void main() {
  auto client = [](const std::string& id){
    using BSO = sheNet::basic_socket_operations;

    int client_fd = BSO::socket();
    /* connect */
    while (true) {
      try {
        BSO::connect(client_fd, "192.168.101.138", "9981");
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

    using io = sheNet::basic_io_operations::TCP;
    while (true) {
      try {
        static int message_num = 0;
        io::send(client_fd, "client " + id + ":No." + std::to_string(++message_num) + " message");
        std::cout << YELLOW_COLOR << "["
                  <<"client " + id + ":No." + std::to_string(++message_num) + " message"
                  <<"]has been sent.\n" << RESET_COLOR;
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
  };
  std::future<void> c1 = std::async(std::launch::async,client,"1");
  std::future<void> c2 = std::async(std::launch::async,client,"2");
  std::future<void> c3 = std::async(std::launch::async,client,"3");
  std::future<void> c4 = std::async(std::launch::async,client,"4");
  std::future<void> c5 = std::async(std::launch::async,client,"5");

  std::thread server([](){
    using BSO = sheNet::basic_socket_operations;
    sheNet::epoll_wrapper wrapper;
    wrapper.set_timeout();
    int server_fd = BSO::socket();/* set */{
    wrapper.add_server_fd(server_fd);
    wrapper.set_timeout(1000);
    BSO::port_reuse(server_fd);
    BSO::set_socket_noblock(server_fd);
    BSO::bind(server_fd,"0.0.0.0","9981");
    BSO::listen(server_fd);
  };

    std::mutex mtx;
    auto accept_thread = std::thread([&server_fd,&wrapper,&mtx](){
      while (true) {
        try {
          std::unique_lock<std::mutex> lock(mtx);
          int client_fd = BSO::accept(server_fd);
          wrapper.add_alive_fd(client_fd);
        }
        catch (const sheNet::sheNetException &exc) {
          if (exc.get_error_code() == 22) {
            sleep(1);
            continue;
          } else {
            std::cout << GREEN_COLOR << "error:" <<exc.what();// 自带\n
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
        auto alive_list = wrapper.get_alive_fd();
        for (auto client_fd : alive_list) {
          std::string get_message = io::recv(client_fd);
          if (get_message.size() > 0) {
            std::cout << GREEN_COLOR << "[" << get_message << "]\n" << RESET_COLOR;
          }
        }
      }
      catch (const sheNet::sheNetException& exc) {
        std::cout << GREEN_COLOR << "recv error" << RESET_COLOR << std::endl;
        sleep(1);
        continue;
      }
    }

    accept_thread.join();
    BSO::shutdown(server_fd);
    std::cout << GREEN_COLOR << "server closed.\n" << RESET_COLOR;
  });

  server.join();
  c1.wait();
  c2.wait();
  c3.wait();
  c4.wait();
  c5.wait();


};

};// namespace m14_epoll_wrapper_test

#endif //SHE_NET_TEST_M14_EPOLL_WRAPPER_TEST_H_
