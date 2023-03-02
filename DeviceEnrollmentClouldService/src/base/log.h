//
// Created by shecannotsee on 2022/10/17.
//
// 该文件是对spdlog的一个简单的封装
// 如何使用?
// 1.设置日志的全局属性,若需要修改日志属性,在此处修改
//   logModule::defaultSetting();
// 2.往spdlog中注册日志模块,若需要修改日志类型,需要修改函数 TODO:注册时需要添加日志目录,现在是写死的
//   logModule::registerLogger("模块名");
// 3.获取已注册的日志模块,并通过返回来写日志
//   auto log_ptr = logModule::getLogger;
//   log_ptr->info("日志内容");
//

#ifndef DEVICEENROLLMENTCLOUDSERVICE_SRC_BASE_LOG_H_
#define DEVICEENROLLMENTCLOUDSERVICE_SRC_BASE_LOG_H_

#include <string>
#include <unordered_set>
#include "spdlog/spdlog.h"
#include "spdlog/spdlog-inl.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace logModule {
using loggerPtr = std::shared_ptr<spdlog::logger>;
static std::unordered_set<std::string> _logger_list;

static void defaultSetting() {
  spdlog::set_level(spdlog::level::trace);// need to set global level and logger can write
  if (true)// just flush in real time
    spdlog::flush_on(spdlog::level::info);
//    spdlog::flush_every(std::chrono::nanoseconds(1));
  else// Timed to write from the buffer like one second interval
    spdlog::flush_every(std::chrono::seconds(1));// Timed to write from the buffer like one second interval
};

static void registerLogger(const std::string &module_name) {
  const std::string fileName = "logs/DECSlog.txt";
  _logger_list.insert(module_name);
  auto logger = spdlog::daily_logger_format_mt(module_name, fileName, 0, 0);
};

static loggerPtr getLogger(const std::string &module_name) {
  if (_logger_list.count(module_name) == 1)
    return spdlog::get(module_name);
  else
    return nullptr;
};

};// namespace log


#endif //DEVICEENROLLMENTCLOUDSERVICE_SRC_BASE_LOG_H_
