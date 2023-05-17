//
// Created by shecannotsee on 2023/3/2.
//

#ifndef SHE_NET_SRC_SHENETEXCEPTION_SHENETEXCEPTION_H_
#define SHE_NET_SRC_SHENETEXCEPTION_SHENETEXCEPTION_H_

#include <exception>
#include <string>

namespace sheNet {

class sheNetException : public std::exception {
 public:
  sheNetException(int error_code,
                  const std::string &error_message);

 private:
  int error_code_;
  std::string error_message_;

 public:
  virtual const char *what() const noexcept override;

  int get_error_code() const;

};// class sheNetException

};// namespace sheNet

#endif //SHE_NET_SRC_SHENETEXCEPTION_SHENETEXCEPTION_H_
