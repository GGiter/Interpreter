#pragma once
#include <string>
#include <optional>
#include "Value.h"

class Variable {
public:
  explicit Variable(const std::string &inName);
  explicit Variable(std::unique_ptr<Value> inValue);
  std::string toString() const;
  const std::optional<std::string> &getName() const;
  const Value *getValue() const;

private:
  std::optional<std::string> name;
  std::unique_ptr<Value> value;
};
