#pragma once
#include "Instruction.h"
#include "Case.h"
#include <memory>
#include <vector>

class Expression;

class Match : public Instruction {
public:
  explicit Match(std::unique_ptr<Expression> inExpression);
  void addCase(std::unique_ptr<Case> inCase);
  std::string toString() const;
  const std::vector<std::unique_ptr<Case>> &getCases() const;
  const Expression *getExpression() const;
  virtual std::optional<ValueType>
  accept(VisitorInterpreter &inVisitor) const override;

private:
  std::vector<std::unique_ptr<Case>> cases;
  std::unique_ptr<Expression> expression;
};
