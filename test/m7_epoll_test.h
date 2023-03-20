//
// Created by shecannotsee on 2023/3/10.
//

#ifndef SHE_NET_TEST_M7_EPOLL_TEST_H_
#define SHE_NET_TEST_M7_EPOLL_TEST_H_

#include <string>
#include <atomic>
#include <socket.h>
#include <sys/epoll.h>

namespace m7_epoll_test {

int main() {
  std::string ip = "192.168.1.47";
  std::string port = "9981";

  sheNet::socket server_socket; /* set and listen */ {
    server_socket.bind(port);
    server_socket.listen();
  };

  int epoll_container_id = epoll_create(1);
  epoll_event events[100000];/* add server fd */ {
    epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = server_socket.get_source_id();
    epoll_ctl(epoll_container_id, EPOLL_CTL_ADD, server_socket.get_source_id(), &ev);
  };

  auto client = [&](std::string data) {
    try {
      sheNet::NetTransport tcp = sheNet::NetTransport::TCP_IPV4;
      std::unique_ptr<sheNet::socket> client = sheNet::CPP11::make_unique<sheNet::socket>(tcp);/* tcp init */ {
        client->connect(ip, port);
      };
      sheNet::message message_control(std::move(client));
      std::cout << "client set done.\n";
      int i = 3;
      while (i--) {
        message_control.send("hello world"+data);
        std::cout << "send done." + data <<"\n";
        sleep(2);
      }
    } catch (const std::exception& exc) {
      std::cout<<exc.what()<<std::endl;
    }
  };
  auto client_future1 = std::async(std::launch::async,client,"1");
  auto client_future2 = std::async(std::launch::async,client,"2");
  auto client_future3 = std::async(std::launch::async,client,"3");
  auto client_future4 = std::async(std::launch::async,client,"4");

  std::atomic<int> client_num;
  while (1) {
    int how_many_event_to_del = epoll_wait(epoll_container_id, events,100000,-1);
    for( int i=0;i<how_many_event_to_del; ++i) {
      int fd = events[i].data.fd;
      auto type = events[i].events;

      if ( fd == server_socket.get_source_id() && (type == EPOLLIN)) {
        server_socket.accept();
        epoll_event temp;
        temp.events = EPOLLIN | EPOLLET; //set ET
        temp.data.fd = server_socket.get_destination_id();
        epoll_ctl(epoll_container_id, EPOLL_CTL_ADD, server_socket.get_destination_id(), &temp);
        printf("accept : %d\n", ++client_num);
      }
      else if ( type == EPOLLIN ) {
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


      }
      else if ( type == EPOLLOUT ) {
        printf("EPOLLOUT happened:[%d].\n",events[i].data.fd);
      }

    }

  };// while (1)


  close(epoll_container_id);
  return 0;
};

};// namespace m7_epoll_test

#endif //SHE_NET_TEST_M7_EPOLL_TEST_H_
