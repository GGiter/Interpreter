#include "Case.h"

Case::Case(std::unique_ptr<Expression> inExpression,
           std::unique_ptr<Block> inBlock)
    : expression(std::move(inExpression)), block(std::move(inBlock)) {}

std::string Case::toString() const { 
    return "case " + expression->toString() + ":" + block->toString();
}

const Expression *Case::getExpression() const { return expression.get(); }

const Block *Case::getBlock() const { return block.get(); }

std::optional<ValueType> Case::accept(VisitorInterpreter &inVisitor) const {
  return inVisitor.visit(*this);
}
