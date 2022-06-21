#pragma once
#include "Variable.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include "../interpreter/VisitorInterpreter.h"

class Expression {
public:
  enum class Operator : int {
    Sum,
    Substraction,
    Multiplication,
    Division,
    Modulo,
    LogicalOr,
    LogicalAnd,
    Less,
    LessEqual,
    More,
    MoreEqual,
    Equal,
    NotEqual,
    Negation,
  };

  virtual ~Expression() = default;
  virtual std::string toString() const = 0;
  virtual std::optional<ValueType>
  accept(class VisitorInterpreter &inVisitor) const = 0;

protected:
  static std::unordered_map<Operator, std::string> tokenMap;
};