//
// Created by shecannotsee on 2022/10/21.
//

#ifndef DEVICEENROLLMENTCLOUDSERVICE_SRC_NET_PACKAGEINSTREAM_H_
#define DEVICEENROLLMENTCLOUDSERVICE_SRC_NET_PACKAGEINSTREAM_H_

#include <string>
#include <list>
#include <mutex>

class packageInStream {
 public:
  enum {HEADER=1,DATA=2,DATAEND=3};
  struct header {
    unsigned int flag   : 1;// 0-数据结尾的包
    unsigned int length : 31;// 1-数据长度
  };
  struct half_data {
    int type;// 0-干净的缓冲区,1-header,2-just data,3-是数据末尾
    int length;// type对应长度,表示从下一个流中要读取的数据长度
    std::string content;// 实际数据存放位置
  };
 private:
  uint64_t _package_length;// 分包长度
  std::list<std::string> _data_list;// 从流中取的完整数据包
  half_data _half_of_data;// 流数据(半截)存储位置
  std::mutex _mutex_out;// 处理拆包时候所用到的锁
 public:
  packageInStream() = delete;
  // _分包长度
  explicit packageInStream(uint64_t package_length);
  ~packageInStream() = default;
  // Copying is prohibited(because std::mutex)
  packageInStream(const packageInStream& other) = delete;
  packageInStream& operator=(const packageInStream& other) = delete;
  // Movement is prohibited(because std::mutex)
  packageInStream(packageInStream&& other) = delete;
  packageInStream& operator=(packageInStream&& other) = delete;
 public:
  // return package(maybe like h-d1-h-d2-h-d3......),input data(=d1+d2+d3+...)
  std::string packing(const std::string& data);
  // input data stream(package)
  void unpacking(const std::string& headWithData);
  // get available data
  bool getPackage(std::string& str);

};

#endif //DEVICEENROLLMENTCLOUDSERVICE_SRC_NET_PACKAGEINSTREAM_H_
