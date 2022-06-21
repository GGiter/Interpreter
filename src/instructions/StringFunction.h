#pragma once
#include "Function.h"
class StringFunction : public Function {
public:
  StringFunction();
  virtual std::optional<ValueType>
  accept(VisitorInterpreter &inVisitor) const override;
};
