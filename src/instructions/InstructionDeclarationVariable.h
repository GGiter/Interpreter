#pragma once
#include "Instruction.h"
#include "Expression.h"
#include <optional>
#include <string>

class InstructionDeclarationVariable : public Instruction {

public:
  explicit InstructionDeclarationVariable(
      const std::string &inIdentifier, bool bInIsMutable,
      std::unique_ptr<Expression> inExpression = nullptr);
  const std::string &getIdentifier() const;
  std::string toString() const;
  bool isMutable() const;
  Expression *getExpression() const;
  virtual std::optional<ValueType>
  accept(VisitorInterpreter &inVisitor) const override;

private:
  std::string identifier;
  bool bIsMutable;
  std::unique_ptr<Expression> expression;
};
