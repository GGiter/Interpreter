#pragma once
#include "Expression.h"
class VariableExpression : public Expression {
public:
  explicit VariableExpression(std::unique_ptr<Variable> inVariable);
  virtual std::string toString() const;
  const Variable *getVariable() const;
  virtual std::optional<ValueType>
  accept(class VisitorInterpreter &inVisitor) const override;

private:
  std::unique_ptr<Variable> variable;
};
