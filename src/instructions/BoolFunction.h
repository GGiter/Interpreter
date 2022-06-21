#pragma once
#include "Function.h"
class BoolFunction : public Function {
public:
  BoolFunction();
  virtual std::optional<ValueType>
  accept(VisitorInterpreter &inVisitor) const override;
};
