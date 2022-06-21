#include "VariableExpression.h"
#include "../interpreter/VisitorInterpreter.h"

VariableExpression::VariableExpression(std::unique_ptr<Variable> inVariable)
    : variable(std::move(inVariable)) {}

std::string VariableExpression::toString() const {
  return variable->toString();
}

const Variable *VariableExpression::getVariable() const {
  return variable.get();
}

std::optional<ValueType> VariableExpression::accept(VisitorInterpreter &inVisitor) const {
  return inVisitor.visit(*this);
}
