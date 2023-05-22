//
// Created by shecannotsee on 23-5-22.
//

#ifndef SHE_NET_TEST_M15_STREAM_DATA_TEST_H_
#define SHE_NET_TEST_M15_STREAM_DATA_TEST_H_

#include <string>
#include <iostream>
#include <thread>
#include <stream_data_parsing/packageInStream.h>
#include <stream_data_parsing/packageInStreamBase64.h>

namespace m15_stream_data_test{

// header with data
namespace method1 {
using std::cout;
using std::endl;
void main() {
  packageInStream a(1024);
  std::string data = "hello world";
  cout << "[" << data << "]" << "is packing...(head with package)" << endl;
  std::string headWithPackage = a.packing(data);
  std::unique_ptr<char> t(new char(headWithPackage.size() + 1));
  (t.get())[headWithPackage.size()] = '\0';
  headWithPackage.copy(t.get(), headWithPackage.size(), 0);

  a.unpacking(headWithPackage);
  std::string recv_str;
  a.getPackage(recv_str);
  int len = recv_str.size();
  cout << "[" << recv_str << "]" << "has received...(head with package)" << endl;
};
};//namespace method1

// base64 data, pass the test
namespace method2 {
using std::cout;
using std::endl;
void main() {
  packageInStreamBase64 a("?");
  std::string data = "hello world";
  cout << "[" << data << "]" << "is packing...(base64)" << endl;
  std::string base64Package = a.packing(data);

  a.unpacking(base64Package);
  std::string recv_str;
  a.getPackage(recv_str);
  cout << "[" << recv_str << "]" << "has received...(base64)" << endl;
};
};//namespace method2

};// namespace m15_stream_data_test

#endif //SHE_NET_TEST_M15_STREAM_DATA_TEST_H_
