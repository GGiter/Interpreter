#include "BinaryExpression.h"

BinaryExpression::BinaryExpression(std::unique_ptr<Expression> inLhs,
                                   Operator inOperator,
                                   std::unique_ptr<Expression> inRHS)
    : lhs(std::move(inLhs)), op(inOperator), rhs(std::move(inRHS)) {}

std::string BinaryExpression::toString() const {
  return lhs->toString() + tokenMap[op] + rhs->toString();
}

const Expression *BinaryExpression::getRhs() const { return rhs.get(); }

const Expression *BinaryExpression::getLhs() const { return lhs.get(); }

const Expression::Operator BinaryExpression::getOperator() const { return op; }

std::optional<ValueType> BinaryExpression::accept(VisitorInterpreter &inVisitor) const {
  return inVisitor.visit(*this);
}
