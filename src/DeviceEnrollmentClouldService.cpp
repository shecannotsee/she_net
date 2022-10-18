//
// Created by shecannotsee on 2022/10/12.
//
#include <iostream>
#include "method_test.h"
#include "server/server.h"

class S {
 public:
  std::vector<int> _data;
  S():_data(1000){};
};

int main() {
  std::cout<<"server start11211.\n";
  std::cout<<"111"<<std::endl;
  S a;
  std::cout<<a._data.size();

  std::cout<<"111"<<std::endl;
  server application;
  application.start();

  std::cout<<"server stop.\n";
  return 0;
};
