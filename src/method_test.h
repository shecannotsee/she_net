//
// Created by shecannotsee on 2022/10/14.
//

#ifndef DEVICEENROLLMENTCLOUDSERVICE_SRC_METHOD_TEST_H_
#define DEVICEENROLLMENTCLOUDSERVICE_SRC_METHOD_TEST_H_

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>//htons,inet_addr
#include <sys/epoll.h>
#include <unistd.h>//close
#include <fcntl.h>//fcntl


namespace method1 {

static int client_num = 0;

void main() {
  struct sockaddr_in serverAddr;
  serverAddr.sin_family = PF_INET;//ipv4
  serverAddr.sin_port = htons(9981);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  int server_socket_id = socket(PF_INET, SOCK_STREAM, 0);
  int bind_result = bind(server_socket_id, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    if (bind_result==-1)  std::cout<<"bind error.\n";
  int listen_result = listen(server_socket_id, 5);
    if (listen_result==-1) std::cout<<"listen error.\n";
  fcntl(server_socket_id, F_SETFL, O_NONBLOCK); // set noblock

  int epoll_container_id = epoll_create(1);
  epoll_event events[5000];
  {
    epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = server_socket_id;
    epoll_ctl(epoll_container_id, EPOLL_CTL_ADD, server_socket_id, &ev);// 包含listen
  }

  bool adasdad = true;
  while (adasdad) {
    int how_many_event_to_del = epoll_wait(epoll_container_id, events,5000,-1);
    for( int i=0;i<how_many_event_to_del; ++i) {
      int fd = events[i].data.fd;
      auto type = events[i].events;

      if ( fd == server_socket_id && (type == EPOLLIN)) {
        int client_fd = accept(server_socket_id, nullptr, NULL);
        epoll_event temp;
        temp.events = EPOLLIN | EPOLLET; //set ET
        temp.data.fd = client_fd;
        epoll_ctl(epoll_container_id, EPOLL_CTL_ADD, client_fd, &temp);
        printf("accept : %d\n", ++client_num);
      } else if ( type == EPOLLIN ) {
        int fd = events[i].data.fd;
        char buffer[1025];
        std::string display;
        bool loopOn = true;
        while (loopOn) {
          memset(buffer,0x00, sizeof(buffer));
          int len = recv(fd, buffer, 1025, NULL);
          if (/* client quit */len == 0||
              /* error */len == -1 ||
              /* buffer边界处理 */len<1025 ) loopOn = false;
          for (int add_index=0; add_index<len; ++add_index)
            display += buffer[add_index];
        }
        std::cout <<fd<< " get data : //[" << display << "]//" <<std::endl;

        int send_len = send(fd, display.c_str(), display.size(), NULL);
        if (send_len==-1)
          printf("send message error.\n");
        else
          printf("send message success.\n");


      } else if ( type == EPOLLOUT ) {
        printf("EPOLLOUT happened:[%d].\n",events[i].data.fd);
      }
    }
    adasdad = false;
  };

  close(epoll_container_id);
  close(server_socket_id);

}

};// namespace method1

#endif //DEVICEENROLLMENTCLOUDSERVICE_SRC_METHOD_TEST_H_
