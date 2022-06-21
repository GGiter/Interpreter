#include "While.h"
#include "Expression.h"
#include "Block.h"
#include "../interpreter/VisitorInterpreter.h"

While::While(std::unique_ptr<Expression> inExpression,
             std::unique_ptr<Block> inBody)
    : expression(std::move(inExpression)), body(std::move(inBody)) {}

std::string While::toString() const {
  return "while(" + expression->toString() + ")" + body->toString();
}

const Expression *While::getExpression() const { return expression.get(); }

const Block *While::getBody() const { return body.get(); }

std::optional<ValueType> While::accept(VisitorInterpreter &inVisitor) const {
  return inVisitor.visit(*this);
}
