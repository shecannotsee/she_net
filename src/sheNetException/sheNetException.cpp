//
// Created by shecannotsee on 2023/2/27.
//

#include "sheNetException.h"

sheNet::sheNetException::sheNetException(int error_code, const std::string& error_message)
    : error_code_(error_code),
      error_message_(std::move(std::string("sheNet error["+std::to_string(error_code_)+"]"+error_message+"\n"))) {
};

const char *sheNet::sheNetException::what() const noexcept {
  return error_message_.c_str();
};

int sheNet::sheNetException::get_error_code() const {
  return error_code_;
}