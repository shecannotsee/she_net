//
// Created by shecannotsee on 2022/10/12.
//
#include <iostream>
#include "base/log.h"
#include "server/server.h"
#include "method_test.h"

int main() {
  logModule::defaultSetting();
  logModule::registerLogger("main");
  auto logger = logModule::getLogger("main");
  logger->info("An application named DECS has been started.");

  server application;
  application.start();

  logger->info("DECS has stopped.");
  return 0;
};
