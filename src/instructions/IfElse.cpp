#include "IfElse.h"
#include "../interpreter/VisitorInterpreter.h"

IfElse::IfElse(std::unique_ptr<Expression> inExpression,
               std::unique_ptr<Block> inBlockIf,
               std::unique_ptr<Block> inBlockElse)
    : expression(std::move(inExpression)), blockIf(std::move(inBlockIf)),
      blockElse(std::move(inBlockElse)) {}

std::string IfElse::toString() const {
  std::string result =
      "if(" + expression->toString() + ")" + blockIf->toString();
  if (blockElse.get()) {
    result += "else" + blockElse->toString();
  }
  return result;
}

const Expression *IfElse::getExpression() const { return expression.get(); }

const Block *IfElse::getBlockIf() const { return blockIf.get(); }

const Block *IfElse::getBlockElse() const { return blockElse.get(); }

std::optional<ValueType> IfElse::accept(VisitorInterpreter &inVisitor) const {
  return inVisitor.visit(*this);
}
