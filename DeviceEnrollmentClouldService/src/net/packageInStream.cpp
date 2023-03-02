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
  _half_of_data.type = NULL;
  _half_of_data.length = 0;
  _half_of_data.content = std::string();
};

std::string packageInStream::packing(const std::string &data) {
  /* data analysis */
  int package_s = data.size()/_package_length + 1;// 分的包个数,+1表示的是最后一个包
  uint64_t last_package_length = data.size()%_package_length;// 最后一个包的长度
  /* packing */
  std::unique_ptr<char> buffer(new char(package_s*_package_length));
  int pos_buffer = 0; // 处于buffer上的游标
  int pos_data = 0; // 处于data上的游标
  header _header;/* update header */{
    _header.flag = 1;
    _header.length = _package_length;
  }
  for (int i=0; i<package_s; i++) {
    if ( i == (package_s-1) ) /* 最后一个包的处理 */ {
      _header.flag = 0;
      _header.length = last_package_length;
    }// 10110000 000000000 00000000 0000
    memcpy(buffer.get()+pos_buffer,&_header, sizeof(header));
    pos_buffer += sizeof(header);
    memcpy(buffer.get()+pos_buffer,data.c_str()+pos_data, _header.length);
    pos_buffer += _header.length;
    pos_data += _header.length;
  }
  /* return */
  std::string ret(buffer.get(),pos_buffer);
  return ret;
};

// TODO:This function does not work properly, it always gives wrong results
void packageInStream::unpacking(const std::string& headWithData) {
  std::string ret;
  int index = 0;// 在headWithData中的处理位置
  header head; /* head init */ {
    head.flag = 0;
    head.length = 0;
  };
  std::unique_ptr<char> data_temp(new char(_package_length));
  // 优先处理_half_of_data
  /**/ if ( _half_of_data.type == NULL    ) {
    constexpr int ssssdasdada = 31415926;
  }
  else if ( _half_of_data.type == HEADER  ) {
    index = index - _half_of_data.length;
    memcpy(&head,_half_of_data.content.c_str(),_half_of_data.length);
  }
  else if ( _half_of_data.type == DATA    ) {
    index = index - _half_of_data.length;
    head.flag = 1;
    head.length = _half_of_data.length;
    ret += _half_of_data.content;
    goto data;
  }
  else if ( _half_of_data.type == DATAEND ) {
    index = index - _half_of_data.length;
    head.flag = 0;
    head.length = _half_of_data.length;
    ret += _half_of_data.content;
    goto data;
  }

  // 再继续读取headWithData里的内容
  while (true) {
    if ( (index == (headWithData.size()-1)) || headWithData.empty() ) /* 抵达流边界时跳出循环 */ {
      _half_of_data.type = 0;
      break;
    };

    if ((index+ sizeof(header)) >= headWithData.size()) /* header越界,需要存储到_half_of_data,并标明长度和大小 */ {
      _half_of_data.content += ret; // 将尚未添提交到队列的数据进行缓存,因为即将break
      _half_of_data.type = HEADER;
      _half_of_data.length = index + sizeof(header) - headWithData.size();
      memcpy(data_temp.get(),headWithData.c_str()+index,headWithData.size()-index); // 拷贝剩余的数据
      _half_of_data.content += std::string(data_temp.get(),headWithData.size()-index);
      break;
    }
    else /* 读取header时候未越界 */ {
      /* 从流中获取header信息,并更新index */ {
        memcpy(&head, headWithData.c_str() + index, sizeof(header));// 拷出head
        index = index + sizeof(header);// 往后偏移一个head大小
      };
data:
      if ( index+head.length >= headWithData.size()) /* data读取时越界,需要存储到_half_of_data,此时拥有head的信息 */ {
        _half_of_data.content += ret; // 将尚未添提交到队列的数据进行缓存,因为即将break
        head.flag == 1 ? _half_of_data.type = DATA : _half_of_data.type = DATAEND;
        _half_of_data.length =  index+head.length - headWithData.size();
        memcpy(data_temp.get(),headWithData.c_str()+index,headWithData.size()-index); // 拷贝剩余的数据
        _half_of_data.content += std::string(data_temp.get(),headWithData.size()-index);
        break;
      }
      else /* 读取data时未越界 */ {
        /* 从流中获取data,并更新index */ {
          memcpy(data_temp.get(), headWithData.c_str() + index, head.length);// 根据header拷出data
          index = index + head.length;// 往后偏移一个读取的data的大小
        };
        /* 获取data的完整性校验 */ {
          ret += std::string(data_temp.get(), head.length);
          if (head.flag == 0) {
            _data_list.emplace_back(ret);
            ret.clear();
          };
        };
      }
    }

  }// while (true)

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


