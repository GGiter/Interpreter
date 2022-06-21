#include "InterpreterValue.h"

InterpreterValue::InterpreterValue(
    Token::Type inType, std::variant<std::string, float, int, bool> inValue,
    bool bInIsMutable)
    : type(inType), value(std::move(inValue)), bIsMutable(bInIsMutable) {}

const std::variant<std::string, float, int, bool> &
InterpreterValue::getValue() const {
  return value;
}

Token::Type InterpreterValue::getType() const { return type; }

bool InterpreterValue::isMutable() const { return bIsMutable; }
