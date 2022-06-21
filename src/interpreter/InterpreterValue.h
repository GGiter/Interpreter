#pragma once
#include "../lexer/Token.h"

class InterpreterValue {
public:
  InterpreterValue(
      Token::Type inType, std::variant<std::string, float, int, bool> inValue, bool bInIsMutable);
  const std::variant<std::string, float, int, bool> &getValue() const;
  Token::Type getType() const;
  bool isMutable() const;

private:
  Token::Type type;
  std::variant<std::string, float, int, bool> value;
  bool bIsMutable;
};
