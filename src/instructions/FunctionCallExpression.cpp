#include "FunctionCallExpression.h"

FunctionCallExpression::FunctionCallExpression(
    std::unique_ptr<Instruction> inFunctionCall)
    : functionCall(std::move(inFunctionCall)) {}

std::string FunctionCallExpression::toString() const {
  return functionCall->toString();
}

const Instruction *FunctionCallExpression::getFunctionCall() const {
  return functionCall.get();
}

std::optional<ValueType> FunctionCallExpression::accept(VisitorInterpreter &inVisitor) const {
  return inVisitor.visit(*this);
}
