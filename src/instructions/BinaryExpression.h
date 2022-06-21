#pragma once
#include "Expression.h"

class BinaryExpression : public Expression {
public:
  explicit BinaryExpression(std::unique_ptr<Expression> inLhs,
                            Operator inOperator,
                            std::unique_ptr<Expression> inRhs);

  virtual std::string toString() const;

  const Expression *getRhs() const;
  const Expression *getLhs() const;
  const Operator getOperator() const;
  virtual std::optional<ValueType> accept(VisitorInterpreter &inVisitor) const override;

private:
  std::unique_ptr<Expression> rhs;
  std::unique_ptr<Expression> lhs;
  Operator op;
};
