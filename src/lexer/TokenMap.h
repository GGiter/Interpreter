#pragma once
#include "Token.h"
#include <map>
#include <vector>

class TokenMap {
public:
  TokenMap() = default;

  Token::Type findKeyword(const std::string &inString) const;
  Token::Type findOperator(const std::string &inString) const;
  Token::Type findSymbol(const std::string &inString) const;

  const std::map<std::string, Token::Type> &getKeywords() const;
  const std::map<std::string, Token::Type> &getOperators() const;
  const std::map<std::string, Token::Type> &getSymbols() const;

private:
  static std::map<std::string, Token::Type> keywords;
  static std::map<std::string, Token::Type> operators;
  static std::map<std::string, Token::Type> symbols;
};
