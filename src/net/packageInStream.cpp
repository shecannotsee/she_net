//
// Created by shecannotsee on 2022/10/21.
//

#include "packageInStream.h"
#include <string.h>
#include <string>
#include <iostream>
#include <memory>

packageInStream::packageInStream(const uint64_t package_length)
    : _package_length(package_length),
      _data_list(std::list<std::string>()) {
  _half_of_data.type = 0;
  _half_of_data.length = 0;
  _half_of_data.content = std::string();
};

std::string packageInStream::packing(const std::string &data) {
  /* data analysis */
  int package_s = data.size()/_package_length + 1;// 分的包个数
  uint64_t last_package_length = data.size()%_package_length;// 最后一个包的长度
  /* packing */
  std::unique_ptr<char> buffer(new char(package_s*_package_length));
  header _header;/* update header */{
    _header.flag = 1;
    _header.length = _package_length;
  }
  void* buffer_index = buffer.get();// 通过这种手段控制p的偏移
  for (int i=0; i<package_s; i++) {
    if ( i == (package_s-1) ) {
      _header.flag = 0;
      _header.length = last_package_length;
    }
    // add header
    memcpy(buffer_index,&_header, sizeof(header));
    buffer_index = (char*)buffer_index + sizeof(header);
    // add data
    memcpy(buffer_index,data.c_str() + (i*_package_length) - (_package_length) + (_header.length),
           _header.length);
    buffer_index = (char*)buffer_index +_header.length;
  }
  /* return */
  std::string ret(buffer.get(),package_s*_package_length-_package_length+last_package_length);
  return ret;
};

void packageInStream::unpacking(const std::string& headWithData) {

};

bool packageInStream::getPackage(std::string &str) {
  if (!_data_list.empty()) {
    std::unique_lock<std::mutex> lock(_mutex_out);
    str = std::move(_data_list.front());
    _data_list.pop_front();
    return true;
  } else {
    return false;
  };
}


