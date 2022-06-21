#pragma once
#include "Expression.h"
class UnaryExpression : public Expression {
public:
  explicit UnaryExpression(Operator inOperator,
                           std::unique_ptr<Expression> inExpression);
  virtual std::string toString() const;
  const Expression *getExpression() const;
  Operator getOperator() const;
  virtual std::optional<ValueType> accept(class VisitorInterpreter &inVisitor) const override;

private:
  std::unique_ptr<Expression> expression;
  Operator op;
};
