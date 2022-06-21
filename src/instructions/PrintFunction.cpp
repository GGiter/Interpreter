#include "PrintFunction.h"

PrintFunction::PrintFunction() : Function("print") {}

std::optional<ValueType>
PrintFunction::accept(VisitorInterpreter &inVisitor) const {
  return inVisitor.visit(*this);
}
