//
// Created by shecannotsee on 2022/10/17.
//
// socket_base是基础的socket类
// 该项目中作为基础的server socket，完成了create socket，bind和listen的作用
// 可以通过getInfo()获取初始化结果
// 可以通过getSocketId()获取listen之后的socket，前提是初始化没有出错

#ifndef DEVICEENROLLMENTCLOUDSERVICE_SRC_NET_SOCKET_BASE_H_
#define DEVICEENROLLMENTCLOUDSERVICE_SRC_NET_SOCKET_BASE_H_

#include <string>
#include <tuple>

class socket_base {
  using message = std::tuple<bool,std::string>;
 private:
  std::string _type;
  int _socket_id;
  message _info;
 public:
  socket_base();
  explicit socket_base(std::string ip,std::string port);
  ~socket_base();
 public:
  std::string getSocketType() const;
  int getSocketId() const;
  message getInfo() const;
};

#endif //DEVICEENROLLMENTCLOUDSERVICE_SRC_NET_SOCKET_BASE_H_
