#include "InstructionReturn.h"

InstructionReturn::InstructionReturn(
    std::unique_ptr<Expression> inExpression)
    : expression(std::move(inExpression)) {}

std::string InstructionReturn::toString() const {
  std::string result = "return";
  if (expression) {
    result += " " + expression->toString();
  }
  result += ";";
  return result;
}

const Expression *InstructionReturn::getExpression() const {
  return expression.get();
}

std::optional<ValueType> InstructionReturn::accept(VisitorInterpreter &inVisitor) const {
  return inVisitor.visit(*this);
}
