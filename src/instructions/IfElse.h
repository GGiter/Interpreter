#pragma once
#include "Block.h"
#include "Expression.h"
#include "Instruction.h"
#include <memory>


class IfElse : public Instruction {
public:
  explicit IfElse(std::unique_ptr<Expression> inExpression,
                  std::unique_ptr<Block> inBlockIf,
                  std::unique_ptr<Block> inBlockElse);
  std::string toString() const;
  const Expression *getExpression() const;
  const Block *getBlockIf() const;
  const Block *getBlockElse() const;
  virtual std::optional<ValueType>
  accept(VisitorInterpreter &inVisitor) const override;

private:
  std::unique_ptr<Expression> expression;
  std::unique_ptr<Block> blockIf;
  std::unique_ptr<Block> blockElse;
};
