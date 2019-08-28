//
// Created by wuyua on 2019-07-07.
//

#ifndef SIMEX_BLOCK_EXCEPTIONS_H
#define SIMEX_BLOCK_EXCEPTIONS_H

#include <sstream>
#include <utility>

class KeyError : public std::exception {
public:
    void *blockPointer;

    KeyError(void *blockPointer);

    const char *what() const noexcept override ;
};
class RuntimeError : public std::exception {
 public:
  std::string msg;

  explicit RuntimeError(std::string msg): msg(std::move(msg)){}

  const char *what() const noexcept override {
      return msg.c_str();
  }
};



#endif //SIMEX_BLOCK_EXCEPTIONS_H
