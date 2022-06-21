#pragma once
#include "Function.h"
class FloatFunction : public Function {
public:
  FloatFunction();
  virtual std::optional<ValueType>
  accept(VisitorInterpreter &inVisitor) const override;
};
