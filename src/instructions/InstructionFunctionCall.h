#pragma once
#include "Instruction.h"
#include "Expression.h"
#include <string>
#include <memory>
#include <vector>

class InstructionFunctionCall : public Instruction {
public:
  explicit InstructionFunctionCall(const std::string &inName);
  void addArgument(std::unique_ptr<Expression> inExpression);
  void setArguments(std::vector<std::unique_ptr<Expression>> inExpressions);
  std::string toString() const;
  const std::string &getFunctionName() const;
  const std::vector<std::unique_ptr<Expression>> &getExpressions() const;
  virtual std::optional<ValueType>
  accept(VisitorInterpreter &inVisitor) const override;

private:
  std::string name;
  std::vector<std::unique_ptr<Expression>> expressions;
};
