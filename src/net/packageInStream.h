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
  struct header {
    unsigned int flag   : 1;
    unsigned int length : 31;
  };
 private:
  header _header;
  header _unpacking_header;
  uint64_t _max;
  std::list<std::string> _data_list;
  std::string _half_data;
  int _half_data_type; // 存储的是header还是data,0为初始状态,-1为header,正数为需要从下一个stream中获取的Bytes
  std::mutex _mutex_out;
  std::mutex _mutex_to_package;
 public:
  packageInStream() = delete;
  explicit packageInStream(uint64_t max);
  ~packageInStream() = default;
  // copy
  packageInStream(const packageInStream& other) = default;
  packageInStream& operator=(const packageInStream& other) = delete;
  // move
  packageInStream(packageInStream&& other) = default;
  packageInStream& operator=(packageInStream&& other) = default;
 public:
  // return package(maybe like h-d1-h-d2-h-d3......),input data(=d1+d2+d3+...)
  std::string packing(const std::string& data);
  // input data stream(package)
  void unpacking(const std::string& headWithData);
  // get available data
  bool getPackage(std::string& str);

};

#endif //DEVICEENROLLMENTCLOUDSERVICE_SRC_NET_PACKAGEINSTREAM_H_
