//
// Created by shecannotsee on 2022/10/24.
//

#include "packageInStreamBase64.h"
#include <string>
#include <list>
#include <memory>
#include <mutex>
#include "base64.h"

packageInStreamBase64::packageInStreamBase64(const std::string& rule)
    : _data_list(std::list<std::string>()),
      _half_data(std::string()),
      _rule(rule) {};

packageInStreamBase64::~packageInStreamBase64() {};

std::string packageInStreamBase64::packing(const std::string &data) {
  std::string ret = data;// TODO:需要添加base64库: std::string ret = sheBase64::encode(data);
  ret = ret + _rule;
  return ret;
};


void packageInStreamBase64::unpacking(const std::string &base64WithEnd) {
  std::string data;
  for (int i=0; i<_half_data.size(); i++) {
    if (_half_data[i]==_rule[0]) {
      _data_list.emplace_back(data);
      data.clear();
      i = i+ _rule.size();
    } else {
      data = data + _half_data[i];
    }
  }
  for (int i=0; i<base64WithEnd.size(); i++) {
    if (base64WithEnd[i]==_rule[0]) {
      std::unique_lock<std::mutex> lock(_mutex_out);
      _data_list.emplace_back(data);
      data.clear();
      i = i+ _rule.size();
    } else {
      data = data + base64WithEnd[i];
    }
  }
  _half_data.clear();
  if (data.size()>0) {
    _half_data += data;
  }
};

bool packageInStreamBase64::getPackage(std::string &str) {
  if (!_data_list.empty()) {
    std::unique_lock<std::mutex> lock(_mutex_out);
    str = std::move(_data_list.front());
    _data_list.pop_front();
    str = "";// TODO:需要添加base64库: str = sheBase64::decode(str);
    return true;
  } else {
    return false;
  };
};


