#pragma once
#include "Function.h"
class IntFunction : public Function {
public:
  IntFunction();
  virtual std::optional<ValueType>
  accept(VisitorInterpreter &inVisitor) const override;
};
