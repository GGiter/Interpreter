#pragma once
#include "../lexer/Token.h"

class Value {
public:
  explicit Value(Token::Type inType, std::variant<std::string, float, int, bool> inValue);
  const std::variant<std::string, float, int, bool> &getValue() const;
  Token::Type getType() const;
  std::string toString() const;
private:
  Token::Type type;
  std::variant<std::string, float, int, bool> value;
};
