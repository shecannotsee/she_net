//
// Created by shecannotsee on 2022/10/17.
//

#ifndef DEVICEENROLLMENTCLOUDSERVICE_SRC_BASE_CONFIG_H_
#define DEVICEENROLLMENTCLOUDSERVICE_SRC_BASE_CONFIG_H_

#include <iostream>
#include <vector>
#include "DataStructure/Section.h"
#include "Parse/Parse.h"

// TODO:可以考虑用可变长参数 or vector 优化该通用函数
// 该函数最多可以读取一个section下有五个k-v键值对
void getConfig(std::string& section, std::string& key1,
                                     std::string& key2,
                                     std::string& key3,
                                     std::string& key4,
                                     std::string& key5) {
  std::vector<sheIni::Section> config;
  /* get config */ {
    sheIni::IniFile config_file("/etc/DeviceEnrollmentCloudService/DeviceEnrollmentCloudService.config");
    config_file.parse(config);
  }
  for (auto& e:config) {
    // get section
    if (e.getSection() == section) {
      for (auto& ee: e.getAllKV() ) {
        if      (std::get<0>(ee) == key1) key1 = std::get<1>(ee);
        else if (std::get<0>(ee) == key2) key2 = std::get<1>(ee);
        else if (std::get<0>(ee) == key3) key3 = std::get<1>(ee);
        else if (std::get<0>(ee) == key4) key4 = std::get<1>(ee);
        else if (std::get<0>(ee) == key5) key5 = std::get<1>(ee);
      }
    }
  }
}

#endif //DEVICEENROLLMENTCLOUDSERVICE_SRC_BASE_CONFIG_H_
