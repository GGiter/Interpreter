#include "InstructionFunctionCall.h"
#include "Variable.h"

InstructionFunctionCall::InstructionFunctionCall(const std::string &inName)
    : name(inName) {}

void InstructionFunctionCall::addArgument(
    std::unique_ptr<Expression> inExpression) {
  expressions.emplace_back(std::move(inExpression));
}

void InstructionFunctionCall::setArguments(
    std::vector<std::unique_ptr<Expression>> inExpressions) {
  expressions = std::move(inExpressions);
}

std::string InstructionFunctionCall::toString() const {
  std::string result = name + "(";
  for (auto &expression : expressions) {
    result += expression->toString();
    if (&expression != &expressions.back())
      result += ",";
  }
  result += ");";
  return result;
}

const std::string &InstructionFunctionCall::getFunctionName() const {
  return name;
}

const std::vector<std::unique_ptr<Expression>> &
InstructionFunctionCall::getExpressions() const {
  return expressions;
}

std::optional<ValueType> InstructionFunctionCall::accept(VisitorInterpreter &inVisitor) const {
  return inVisitor.visit(*this);
}
