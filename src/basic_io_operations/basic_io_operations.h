//
// Created by shecannotsee on 23-5-15.
//

#ifndef SHE_NET_SRC_BASIC_IO_OPERATIONS_BASIC_IO_OPERATIONS_H_
#define SHE_NET_SRC_BASIC_IO_OPERATIONS_BASIC_IO_OPERATIONS_H_

#include <string>
#include <transport_address_type.h>

namespace sheNet {

class basic_io_operations {
 public:
  // copy:off
  basic_io_operations(const basic_io_operations &) = delete;
  basic_io_operations &operator=(const basic_io_operations &) = delete;
  // move:off
  basic_io_operations(basic_io_operations &&) = delete;
  basic_io_operations &operator=(basic_io_operations &&) = delete;
  //destructors
  ~basic_io_operations() = default;
  //constructors
  basic_io_operations() = default;
 private:// data
  // NULL
 public:// interface
  /**
   * @brief 实际上和tcp绑定,用来获取缓冲区中的数据.该接口每次只获取最多1024字节的数据.
   * ps:若缓冲区还有数据的时候客户端已经关闭了,recv先获取缓冲区的数据,在抛出客户端关闭异常
   * @param fd 文件描述符
   * @return 获取到的二进制流,需要进行流数据的切分和处理.若出现系统接口错误则会抛出异常
   */
  static std::string recv(int fd);

  /**
   * @brief 和tcp绑定,用来将数据添加到发送缓冲区.该函数会发送所有的数据.
   * 若缓冲区满在非阻塞下则会持续进行系统接口添加数据到缓冲区;在阻塞情况下则会阻塞在添加上直到数据成功添加才会正常返回
   * @param fd 文件描述符
   * @param binary_stream 需要发送的二进制流(流中应该做了切分处理)
   */
  static void send(int fd ,const std::string &binary_stream);

  /**
   * @brief 和udp绑定,用来获取指定缓冲区中的数据
   * @param fd 文件描述符,该fd需要提前绑定端口
   * @return 返回获取的数据
   */
  static std::string recvfrom(int fd);

  /**
   * @brief udp发送,用来将数据添加到发送缓冲区
   * @param ip 发送的地址ip
   * @param port 发送的地址ip对应的端口
   * @param binary_package 发送的数据包
   */
  static void sendto(std::string ip, std::string port, std::string binary_package);

};

};// namespace sheNet

#endif //SHE_NET_SRC_BASIC_IO_OPERATIONS_BASIC_IO_OPERATIONS_H_
