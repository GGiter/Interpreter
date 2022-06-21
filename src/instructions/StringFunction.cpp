#include "StringFunction.h"

StringFunction::StringFunction() : Function("string") {}

std::optional<ValueType>
StringFunction::accept(VisitorInterpreter &inVisitor) const {
  return inVisitor.visit(*this);
}
