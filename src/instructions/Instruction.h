#pragma once
#include <string>
#include "../interpreter/VisitorInterpreter.h"

class Instruction {
public:
  Instruction() = default;
  virtual ~Instruction() = default;
  virtual std::string toString() const = 0;
  virtual std::optional<ValueType>
  accept(class VisitorInterpreter &inVisitor) const = 0;
};
