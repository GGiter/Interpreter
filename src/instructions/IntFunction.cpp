#include "IntFunction.h"

IntFunction::IntFunction() : Function("int") {}

std::optional<ValueType>
IntFunction::accept(VisitorInterpreter &inVisitor) const {
  return inVisitor.visit(*this);
}
