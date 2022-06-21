#include "InstructionAssigment.h"
#include "Expression.h"
#include "Variable.h"

InstructionAssigment::InstructionAssigment(
    std::unique_ptr<Variable> inVariable,
    std::unique_ptr<Expression> inExpression)
    : variable(std::move(inVariable)), expression(std::move(inExpression)) {}

std::string InstructionAssigment::toString() const {
  return variable->toString() + "=" + expression->toString() + ";";
}

const Variable *InstructionAssigment::getVariable() const {
  return variable.get();
}

const Expression *InstructionAssigment::getExpression() const {
  return expression.get();
}

std::optional<ValueType> InstructionAssigment::accept(VisitorInterpreter &inVisitor) const {
  return inVisitor.visit(*this);
}
