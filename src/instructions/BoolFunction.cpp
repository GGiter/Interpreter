#include "BoolFunction.h"

BoolFunction::BoolFunction() : Function("bool") {}

std::optional<ValueType>
BoolFunction::accept(VisitorInterpreter &inVisitor) const {
  return inVisitor.visit(*this);
}
