#pragma once
#include "Instruction.h"
#include <memory>

class Block;
class Expression;

class While : public Instruction {
public:
  explicit While(std::unique_ptr<Expression> inExpression, std::unique_ptr<Block> inBody);
  std::string toString() const;
  const Expression *getExpression() const;
  const Block *getBody() const;
  virtual std::optional<ValueType>
  accept(class VisitorInterpreter &inVisitor) const override;

private:
  std::unique_ptr<Expression> expression;
  std::unique_ptr<Block> body;
};
