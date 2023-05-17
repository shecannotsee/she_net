//
// Created by shecannotsee on 2023/3/9.
//

#ifndef SHE_NET_SRC_SELECT_SELECT_WRAPPER_H_
#define SHE_NET_SRC_SELECT_SELECT_WRAPPER_H_

#include <vector>
#include <sys/socket.h>

namespace sheNet {

class select_wrapper {
 public:
  // copy:off
  select_wrapper(const select_wrapper&) = delete;
  select_wrapper& operator=(const select_wrapper&) = delete;
  // move:on
  select_wrapper(select_wrapper&&) = default;
  select_wrapper& operator=(select_wrapper&&) = default;
  //destructors
  ~select_wrapper() = default;
  //constructors
  select_wrapper();

 private:// data
  fd_set read_fds_;
  fd_set write_fds_;
  fd_set except_fds_;

  std::vector<int> fd_list_;///< 用于管理连接上的文件描述符
  timeval timeout_;///< 用于设置select的超时

 private:
  /**
   * @brief 获取select监听的描述符的最大值(软件限制)用于select接口的第一个参数
   * @return 返回当前(软件层面)限制的最大值
   */
  static int get_system_max_listen_fd();

 public:// interface
  /**
   * @brief 向select_wrapper所维护的fd列表中添加通过basic_socket_operations::accept获取的fd列表.
   * 含义其实就是将已经连接上的fd放在一个列表中维护,后续通过该列表进行io处理
   * @param fd 需要添加的文件描述符
   */
  void add_alive_fd(int fd);

  /**
   * @brief 去除已经断开连接的fd
   * @param fd 需要删除的文件描述符
   */
  void remove_alive_fd(int fd);

  /**
   * @brief 用于设置select接口的超时时间,可大可小
   * @param seconds 秒数,默认为0
   * @param microseconds 微秒,默认为0
   */
  void set_timeout(int seconds, int microseconds);

};



};// namespace sheNet

#endif //SHE_NET_SRC_SELECT_SELECT_WRAPPER_H_
