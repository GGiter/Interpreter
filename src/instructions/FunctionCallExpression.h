#pragma once
#include "Expression.h"
#include "Instruction.h"
#include "../interpreter/VisitorInterpreter.h"

class FunctionCallExpression : public Expression {
public:
  explicit FunctionCallExpression(std::unique_ptr<Instruction> inFunctionCall);
  virtual std::string toString() const;
  const Instruction *getFunctionCall() const;
  virtual std::optional<ValueType>
  accept(class VisitorInterpreter &inVisitor) const override;

private:
  std::unique_ptr<Instruction> functionCall;
};
