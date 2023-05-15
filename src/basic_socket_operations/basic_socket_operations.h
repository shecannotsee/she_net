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
  static int socket(const TRANSPORT_ADDRESS_TYPE type);

  /**
   * @param fd
   * @param ip
   * @param port
   * @return
   */
  void bind(int fd,std::string ip, std::string port);

  /**
   * @param fd
   * @return
   */
  void listen(int fd);

  /**
   * @param fd
   * @param ip
   * @param port
   * @return
   */
  void connect(int fd, std::string ip, std::string port);

  /**
   * @param fd
   * @return
   */
  int accept(int fd);





};


} // sheNet

#endif //SHE_NET_SRC_BASIC_SOCKET_OPERATIONS_BASIC_SOCKET_OPERATIONS_H_
