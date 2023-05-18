//
// Created by shecannotsee on 23-5-15.
//

#ifndef SHE_NET_SRC_BASIC_SOCKET_OPERATIONS_BASIC_SOCKET_OPERATIONS_H_
#define SHE_NET_SRC_BASIC_SOCKET_OPERATIONS_BASIC_SOCKET_OPERATIONS_H_

#include <string>
#include <transport_address_type.h>

namespace sheNet {

class basic_socket_operations {
 public:
  // copy:off
  basic_socket_operations(const basic_socket_operations&) = delete;
  basic_socket_operations& operator=(const basic_socket_operations&) = delete;
  // move:off
  basic_socket_operations(basic_socket_operations&&) = delete;
  basic_socket_operations& operator=(basic_socket_operations&&) = delete;
  //destructors
  ~basic_socket_operations() = default;
  //constructors
  basic_socket_operations() = default;
 private:// data
  // NULL
 public:// interface
  /**
   * @param type 网络传输类型: TCP/UDP + IPV4/IPV6
   * @return socket fd,若创建失败则会抛出异常
   */
  static int socket(TRANSPORT_ADDRESS_TYPE type = TRANSPORT_ADDRESS_TYPE::TCP_IPV4);

  /**
   * @param fd 需要进行端口绑定的文件描述符
   * @param ip 绑定ip,一般为"0.0.0.0"或者"::"
   * @param port 需要绑定的端口
   * @param type 网络传输类型
   * @return 若端口绑定失败则会抛出异常
   */
  static void bind(int fd,std::string ip, std::string port, TRANSPORT_ADDRESS_TYPE type  = TRANSPORT_ADDRESS_TYPE::TCP_IPV4);

  /**
   * @param fd 需要监听的文教描述符
   * @return 若监听失败则会抛出异常
   */
  static void listen(int fd, int backlog = 5);

  /**
   * @brief 阻塞或者非阻塞需要设置socket
   * @param local_fd 本地的服务的文件描述符
   * @param type 网络传输类型
   * @return 返回连接上的fd.若接受失败或者在非阻塞模式下会频繁的抛出异常,请酌情处理
   */
  static int accept(int local_fd, TRANSPORT_ADDRESS_TYPE type  = TRANSPORT_ADDRESS_TYPE::TCP_IPV4);

  /**
   * @brief 阻塞或者非阻塞需要设置socket
   * @param local_fd 本地创建的fd
   * @param ip 需要连接的ip地址
   * @param port 需要连接的端口号
   * @param type 网络传输类型
   * @return 返回客户端使用的端口号.阻塞情况下连接失败会抛出异常,在非阻塞模式下会频繁的抛出异常
   */
  static int connect(int local_fd, std::string ip, std::string port, TRANSPORT_ADDRESS_TYPE type = TRANSPORT_ADDRESS_TYPE::TCP_IPV4);

  /**
   * @brief 关闭文件描述符
   * @param fd 需要关闭的文件描述符
   */
  static void shutdown(int fd);

 public:
  /**
   * @brief 端口复用设置
   * @param fd 需要处理的文件描述符
   * @param operations 1意味着开启端口复用,0意味着关闭
   * @return 若设置失败则会抛出异常
   */
  static void port_reuse(int fd, int operations = 1);

  /**
   * @brief 设置文件描述符为阻塞还是非阻塞
   * @param fd 文件描述符
   * @param block true意味着非阻塞,false意味着阻塞.调用时默认将socket调整成非阻塞,因为socket创建后默认为阻塞
   * @return 若设置失败则会抛出异常
   */
  static void set_socket_noblock(int fd, bool noblock = true);


};


} // sheNet

#endif //SHE_NET_SRC_BASIC_SOCKET_OPERATIONS_BASIC_SOCKET_OPERATIONS_H_
