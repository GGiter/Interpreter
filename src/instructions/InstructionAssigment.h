#pragma once
#include "Instruction.h"
#include <memory>

class Variable;
class Expression;

class InstructionAssigment : public Instruction {
public:
  explicit InstructionAssigment(std::unique_ptr<Variable> inVariable, std::unique_ptr<Expression> inExpression);
  std::string toString() const;
  const Variable *getVariable() const;
  const Expression *getExpression() const;
  virtual std::optional<ValueType> accept(VisitorInterpreter &inVisitor) const override;

private:
  std::unique_ptr<Variable> variable;
  std::unique_ptr<Expression> expression;
};
