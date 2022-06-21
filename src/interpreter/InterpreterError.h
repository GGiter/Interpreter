#pragma once
#include <stdexcept>

class InterpreterError : public std::runtime_error {
public:
  explicit InterpreterError(const std::string &_Message)
      : std::runtime_error(_Message.c_str()) {}

  explicit InterpreterError(const char *_Message)
      : std::runtime_error(_Message) {}
};
