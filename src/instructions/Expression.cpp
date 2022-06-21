#include "Expression.h"

std::unordered_map<Expression::Operator, std::string> Expression::tokenMap = {
    {Expression::Operator::Sum, "="},
    {Expression::Operator::Substraction, "-"},
    {Expression::Operator::Multiplication, "*"},
    {Expression::Operator::Division, "/"},
    {Expression::Operator::Modulo, "%"},
    {Expression::Operator::LogicalOr, "||"},
    {Expression::Operator::LogicalAnd, "&&"},
    {Expression::Operator::Less, "<"},
    {Expression::Operator::LessEqual, "<="},
    {Expression::Operator::More, ">"},
    {Expression::Operator::MoreEqual, ">="},
    {Expression::Operator::Equal, "=="},
    {Expression::Operator::NotEqual, "!="},
    {Expression::Operator::Negation, "!"},
};
