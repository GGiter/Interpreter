#pragma once

#include "Source.h"
#include "Token.h"
#include "TokenMap.h"
#include <map>
#include <memory>
#include <optional>
#include <vector>

class Lexer {

public:
  Lexer(std::unique_ptr<Source> inSource);

  const Token &getNextToken() const;

private:
  mutable Token currentToken;
  std::optional<Token> tryStringToken(int firstSymbol) const;
  std::optional<Token> tryStringTokenInternal(int firstSymbol, int startSymbol) const;
  std::optional<Token> tryNumberToken(int firstSymbol) const;
  std::optional<Token> tryOperatorToken(int firstSymbol) const;
  std::optional<Token> trySymbolToken(int firstSymbol) const;
  std::optional<Token> tryAlphaToken(int firstSymbol) const;

  void consumeWhiteLines() const;

  std::unique_ptr<Source> source;
  TokenMap tokenMap;
};
