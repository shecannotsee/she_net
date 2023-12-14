//
// Created by shecannotsee on 23-12-14.
//

#ifndef T7_STREAM_DATA_H
#define T7_STREAM_DATA_H

#include <string>
#include <iostream>
#include <thread>
#include <stream_data_parsing/packageInStream.h>
#include <stream_data_parsing/packageInStreamBase64.h>

namespace src_testing::t7_stream_data {

using std::cout;
using std::endl;

std::string main() {
  {
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
  }

  {
    packageInStreamBase64 a("?");
    std::string data = "hello world";
    cout << "[" << data << "]" << "is packing...(base64)" << endl;
    std::string base64Package = a.packing(data);

    a.unpacking(base64Package);
    std::string recv_str;
    a.getPackage(recv_str);
    cout << "[" << recv_str << "]" << "has received...(base64)" << endl;
  }

  return "need to fill";
}

}

#endif //T7_STREAM_DATA_H
