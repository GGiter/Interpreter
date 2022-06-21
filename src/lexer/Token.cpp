#include "Token.h"

Token::Token(const SourcePosition &inStartPosition)
    : startPosition(inStartPosition), type(Token::Type::BadType) {}

void Token::setTokenType(Type inType) { type = inType; }

void Token::setValue(std::variant<std::string, float, int, bool> inValue) {
  value = std::move(inValue);
}

Token::Type Token::getTokenType() const { return type; }

const std::variant<std::string, float, int, bool> &Token::getValue() const {
  return value;
}

const SourcePosition &Token::getStartPosition() const { return startPosition; }

bool Token::operator==(Token::Type InType) const { return type == InType; }
