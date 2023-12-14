//
// Created by shecannotsee on 23-5-17.
//

#ifndef SHE_NET_SRC_POLL_WRAPPER_POLL_WRAPPER_H_
#define SHE_NET_SRC_POLL_WRAPPER_POLL_WRAPPER_H_

#include <sys/poll.h>

#include <vector>

namespace she_net {

class poll_wrapper {
 public:
  // copy:off
  poll_wrapper(const poll_wrapper&) = delete;
  poll_wrapper& operator=(const poll_wrapper&) = delete;
  // move:on
  poll_wrapper(poll_wrapper&&) = default;
  poll_wrapper& operator=(poll_wrapper&&) = default;
  // destructors
  ~poll_wrapper() = default;
  // constructors
  poll_wrapper();

 private:
  std::vector<pollfd> poll_fds_;  ///< poll_wrapper维护的文件描述符列表
  int timeout_set_;               ///< 用于设置poll的的超时时间，单位是毫秒

 public:
  /**
   * @brief 添加监听的服务端fd,也就是local fd.
   * ps:请注意,local_fd应该总是在第一个被添加
   * @param local_fd 服务端文件描述符,用来接收客户端的连接
   */
  void add_server_fd(int local_fd);

  /**
   * @brief 添加通过accept获取的客户端fd
   * @param fd 需要添加的文件描述符
   */
  void add_alive_fd(int fd);

  /**
   * @brief 去除已经断开连接的fd
   * @param fd 需要删除的文件描述符
   */
  void remove_alive_fd(int fd);

  /**
   * @brief 设置超时时间
   * @param milliseconds 超时毫秒数
   */
  void set_timeout(int milliseconds = 0);

  /**
   * @brief 通过调用poll来获取所有可读的文件描述符.
   * 该接口在获取可读fd后会自动将他的 轮询者关心的事件类型 改为可读可写,以便于下次能够继续触发
   * @return 返回所有可用的文件描述符
   */
  std::vector<int> get_alive_fd();
};

};  // namespace she_net

#endif  // SHE_NET_SRC_POLL_WRAPPER_POLL_WRAPPER_H_
