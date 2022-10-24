//
// Created by shecannotsee on 2022/10/21.
//

#include "packageInStream.h"
#include <string.h>
#include <string>
#include <iostream>
#include <memory>

void h(){
  std::string data = "hello world dadsasdsdsa  ddddddddddasssssssss ddddddddd";
  packageInStream::header data_head;/* init */ {
    data_head.flag = 0;
    data_head.length = data.size();
  }
  char buff[1024];/* init */ {
    memset(buff, 0x00, sizeof(buff));
  }
  // 将包头加入到缓冲区
  memcpy(buff+0,&data_head,sizeof(data_head));
  // 将真实数据加入到缓冲区
  memcpy(buff+sizeof(data_head),//param1:从包头结束往后加
         data.c_str()+0,  // param2:数据头(考虑到数据过长可能拆分,故+0)
         data.size());  // param3:实际的数据长度(可能会做拆开发送,所以这里并不一定是数据的总长度)

  /* recv */ {
    packageInStream::header head;
    // 获取此次缓冲区中的包头信息(若拆包,则会在buffer里进行偏移)
    memcpy(&head,buff, sizeof(packageInStream::header));
    char* display = new char[head.length];// 临时的接受数据的变量,若拆包,实际长度会有变化
    // 根据包头获取实际数据并放入接收buffer中
    memcpy(display, //接收buffer
           buff+ sizeof(packageInStream::header), // 真实数据的起始位置
           head.length);// 数据的长度
    std::cout<<"recv : ["<<display<<"]"<<std::endl;
    std::string test(display,head.length); //简单做一个char*到std::string的转换
    delete[] display;display = nullptr;
    std::cout<<"str test:["<<test<<"]"<<std::endl;
  }
};

packageInStream::packageInStream(const uint64_t max)
    : _max(max),
      _data_list(std::list<std::string>()),
      _half_data(std::string()),
      _half_data_type(0) {
  _header.flag = 0;
  _header.length = 0;
  _unpacking_header.flag = 1;
  _unpacking_header.length = 0;
};

std::string packageInStream::packing(const std::string &data) {
  std::unique_lock<std::mutex> lock(_mutex_to_package);
  /* data analysis */
  int package_s = 0;
  uint64_t last_package_length = 0;
  package_s = data.size()/_max + 1;
  last_package_length = data.size()%_max;
  /* packing */
  std::unique_ptr<char> buffer(new char(package_s*_max));
  /* update header */{
    _header.flag = 1;
    _header.length = _max;
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
    memcpy(buffer_index,data.c_str() + (i*_max) - (_max) + (_header.length),
           _header.length);
    buffer_index = (char*)buffer_index +_header.length;
  }
  /* return */
  std::string ret(buffer.get(),package_s*_max-_max+last_package_length);
  return ret;
};

void packageInStream::unpacking(const std::string& headWithData) {
  int len = headWithData.size();
  int index = 0;
  while (true) {
    if (_half_data_type == 0) {
      /* ********* deal with header ********* */
      if ( index + sizeof(header) >= len) { //若要取的header越界
        _half_data_type = -1;
        // 将剩余字节放入_half_data中
        std::unique_ptr<char> p(new char(index+ sizeof(header) -len));
        memcpy(p.get(),headWithData.c_str()+index,index+ sizeof(header) -len);
        _half_data += p.get();
        break;
      } else {
        memcpy(&_unpacking_header,headWithData.c_str()+index, sizeof(header));
        index += sizeof(header);
      }
      /* ********* deal with data   ********* */
      if ( index + _unpacking_header.length >= len ) {
        _half_data_type = index+_unpacking_header.length-len;
        // 将剩余字节放入_half_data中
        break;
      } else {
        if (_unpacking_header.flag == 0)

      }

    } else if (_half_data_type < 0) {

    } else if (_half_data_type > 0) {

    }

  };
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


