#pragma once
#include "Function.h"
class PrintFunction : public Function {
public:
  PrintFunction();
  virtual std::optional<ValueType>
  accept(VisitorInterpreter &inVisitor) const override;
};
