#include "UnaryExpression.h"

UnaryExpression::UnaryExpression(Operator inOperator,
                                 std::unique_ptr<Expression> inExpression)
    : op(inOperator), expression(std::move(inExpression)) {}

std::string UnaryExpression::toString() const {
  return tokenMap[op] + expression->toString();
}

const Expression *UnaryExpression::getExpression() const {
  return expression.get();
}

Expression::Operator UnaryExpression::getOperator() const { return op; }

std::optional<ValueType> UnaryExpression::accept(VisitorInterpreter &inVisitor) const {
  return inVisitor.visit(*this);
}
