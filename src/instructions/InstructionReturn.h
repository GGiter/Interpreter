#pragma once
#include "Expression.h"
#include "Instruction.h"
#include <memory>

class InstructionReturn : public Instruction {
public:
  explicit InstructionReturn(std::unique_ptr<Expression> inExpression = nullptr);
  std::string toString() const;
  const Expression *getExpression() const;
  virtual std::optional<ValueType>
  accept(VisitorInterpreter &inVisitor) const override;

private:
  std::unique_ptr<Expression> expression;
};
