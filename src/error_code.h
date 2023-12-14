//
// Created by shecannotsee on 23-12-14.
//

#ifndef ERROR_CODE_H
#define ERROR_CODE_H

namespace she_net {

enum class ERROR_CODE : int {
  NO_ERROR                 = 0,
  SOCKET_CREATE_ERROR      = 1,
  BIND_ERROR               = 2,
  LISTEN_ERROR             = 3,
  CONNECT_ERROR            = 4,
  ACCEPT_ERROR             = 5,
  TCP_RECV_DATA_ERROR      = 6,
  TCP_SEND_DATA_ERROR      = 7,
  UDP_RECV_DATA_ERROR      = 8,
  UDP_SEND_DATA_ERROR      = 9,
  SET_PORT_REUSE_ERROR     = 10,
  SET_SOCKET_BLOCK_ERROR   = 11,
  CLIENT_CLOSE             = 12,
  RECVFROM_FD_ERROR        = 13,
  SELECT_ERROR             = 14,
  SELECT_WRAPPER_ERROR     = 15,
  POLL_WRAPPER_ERROR       = 16,
  EPOLL_CTL_ADD_ERROR      = 17,
  EPOLL_CTL_REMOVE_ERROR   = 18,
  EPOLL_WAIT_ACCEPT_ERROR  = 19,
  EPOLL_WAIT_FD_ERROR      = 20,
  NO_BLOCK_CONNECT_RUNNING = 21,
  NO_BLOCK_ACCEPT_RUNNING  = 22,
};

}

#endif //ERROR_CODE_H
