#include "FloatFunction.h"

FloatFunction::FloatFunction() : Function("float") {}

std::optional<ValueType>
FloatFunction::accept(VisitorInterpreter &inVisitor) const {
  return inVisitor.visit(*this);
}
