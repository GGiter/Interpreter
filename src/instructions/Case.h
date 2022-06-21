#pragma once
#include <memory>
#include "Instruction.h"
#include "Block.h"
#include "Expression.h"
#include "../interpreter/VisitorInterpreter.h"

class Case : public Instruction {
public:
  explicit Case(std::unique_ptr<Expression> inExpression, std::unique_ptr<Block> inBlock);
  std::string toString() const;

  const Expression* getExpression() const;
  const Block *getBlock() const;
  virtual std::optional<ValueType> accept(VisitorInterpreter &inVisitor) const override;

private:
  std::unique_ptr<Expression> expression;
  std::unique_ptr<Block> block;
};
