#include "InstructionDeclarationVariable.h"

InstructionDeclarationVariable::InstructionDeclarationVariable(
    const std::string &inIdentifier,
    bool bInIsMutable,
    std::unique_ptr<Expression> inExpression)
    : identifier(inIdentifier), bIsMutable(bInIsMutable), expression(std::move(inExpression)) {}

const std::string &InstructionDeclarationVariable::getIdentifier() const {
  return identifier;
}

std::string InstructionDeclarationVariable::toString() const {
  std::string result = "";
  if (bIsMutable) {
    result += "mut ";
  }
  result += "var " + identifier;
  if (expression) {
    result += "=" + expression->toString();
  }
  result += +";";
  return result;
}

bool InstructionDeclarationVariable::isMutable() const { return bIsMutable; }

Expression *InstructionDeclarationVariable::getExpression() const {
  return expression.get();
}

std::optional<ValueType> InstructionDeclarationVariable::accept(VisitorInterpreter &inVisitor) const {
  return inVisitor.visit(*this);
}
