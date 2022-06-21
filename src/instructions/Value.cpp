#include "Value.h"

Value::Value(Token::Type inType,
             std::variant<std::string, float, int, bool> inValue)
    : type(inType), value(std::move(inValue)) {}

const std::variant<std::string, float, int, bool> &Value::getValue() const {
  return value;
}

Token::Type Value::getType() const { return type; }

std::string Value::toString() const {
  if (type == Token::Type::StringLiteral)
    return "\"" + std::get<std::string>(value) + "\"";
  if (type == Token::Type::FloatLiteral)
    return std::to_string(std::get<float>(value));
  if (type == Token::Type::IntLiteral)
    return std::to_string(std::get<int>(value));

  if (std::get<bool>(value) == 1)
    return "true";

  return "false";
}
