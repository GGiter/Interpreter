#include "Variable.h"

Variable::Variable(const std::string &inName)
    : name(inName), value(nullptr) {}

Variable::Variable(std::unique_ptr<Value> inValue)
    : value(std::move(inValue)), name(std::nullopt) {}

std::string Variable::toString() const {
  if (name != std::nullopt)
    return *name;
  return value->toString();
}

const std::optional<std::string> &Variable::getName() const { return name; }

const Value *Variable::getValue() const { return value.get(); }