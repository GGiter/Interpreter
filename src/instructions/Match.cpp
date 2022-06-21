#include "Match.h"
#include "../interpreter/VisitorInterpreter.h"

Match::Match(std::unique_ptr<Expression> inExpression)
    : expression(std::move(inExpression)) {}

void Match::addCase(std::unique_ptr<Case> inCase) {
  cases.emplace_back(std::move(inCase));
}

std::string Match::toString() const {
  std::string result = "match(" + expression->toString() + "){";
  for (auto &c : cases) {
    result += c->toString();
  }
  result += "}";
  return result;
}

const std::vector<std::unique_ptr<Case>> &Match::getCases() const {
  return cases;
}

const Expression *Match::getExpression() const { return expression.get(); }

std::optional<ValueType> Match::accept(VisitorInterpreter &inVisitor) const {
  return inVisitor.visit(*this);
}
