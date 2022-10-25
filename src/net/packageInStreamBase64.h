//
// Created by shecannotsee on 2022/10/24.
//

#ifndef DEVICEENROLLMENTCLOUDSERVICE_SRC_NET_PACKAGEINSTREAMBASE64_H_
#define DEVICEENROLLMENTCLOUDSERVICE_SRC_NET_PACKAGEINSTREAMBASE64_H_

#include <string>
#include <list>
#include <mutex>

class packageInStreamBase64 {
 private:
  std::list<std::string> _data_list;
  std::string _half_data;
  std::string _rule;
  std::mutex _mutex_out;
 public:
  packageInStreamBase64() = delete;
  // input parameter(rule) represents the boundary of the byte stream
  explicit packageInStreamBase64(const std::string& rule);
  ~packageInStreamBase64();
  // Copying is prohibited(because std::mutex)
  packageInStreamBase64(const packageInStreamBase64& other) = delete;
  packageInStreamBase64& operator=(const packageInStreamBase64& other) = delete;
  // Movement is prohibited(because std::mutex)
  packageInStreamBase64(packageInStreamBase64&& other) = delete;
  packageInStreamBase64& operator=(packageInStreamBase64&& other) = delete;
 private:

 public:
  std::string packing(const std::string& data);
  void unpacking(const std::string& base64WithEnd);
  bool getPackage(std::string& str);

};

#endif //DEVICEENROLLMENTCLOUDSERVICE_SRC_NET_PACKAGEINSTREAMBASE64_H_
