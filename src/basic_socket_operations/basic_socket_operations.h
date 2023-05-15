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
   * @param fd
   * @param ip
   * @param port
   * @param type
   * @return 在非阻塞模式下会
   */
  static void connect(int fd, std::string ip, std::string port, TRANSPORT_ADDRESS_TYPE type = TRANSPORT_ADDRESS_TYPE::TCP_IPV4);

  /**
   * @param fd
   * @return
   */
  void shutdown(int fd);



};


} // sheNet

#endif //SHE_NET_SRC_BASIC_SOCKET_OPERATIONS_BASIC_SOCKET_OPERATIONS_H_
