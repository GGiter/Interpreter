#include "Lexer.h"
#include "SourcePosition.h"
#include <cstdio>
#include <stdexcept>

Lexer::Lexer(std::unique_ptr<Source> inSource) : source(std::move(inSource)) {}

const Token &Lexer::getNextToken() const {

  currentToken = Token(source->getCurrentPosition());

  consumeWhiteLines();
  int nextSymbol = source->getNextChar();

  if (nextSymbol == EOF) {
    currentToken.setTokenType(Token::Type::Eof);
  } else {
    if (tryAlphaToken(nextSymbol))
      return currentToken;
    if (tryNumberToken(nextSymbol))
      return currentToken;
    if (tryStringToken(nextSymbol))
      return currentToken;
    if (tryOperatorToken(nextSymbol))
      return currentToken;

    trySymbolToken(nextSymbol);
  }

  return currentToken;
}

std::optional<Token> Lexer::tryStringToken(int firstSymbol) const {
  if (tryStringTokenInternal(firstSymbol, '"')) {
    return currentToken;
  }
  if (tryStringTokenInternal(firstSymbol, '\'')) {
    return currentToken;
  }

  return std::nullopt;
}

std::optional<Token> Lexer::tryStringTokenInternal(int firstSymbol,

                                                   int startSymbol) const {
  if (firstSymbol != startSymbol)
    return std::nullopt;

  std::string result;
  int nextSymbol = source->peekNextChar();
  while (nextSymbol != startSymbol && nextSymbol != EOF) {
    if (source->isFile())
    {
      if (nextSymbol == '\\') {
        source->getNextChar();
        nextSymbol = source->peekNextChar();
        if (nextSymbol != startSymbol && nextSymbol != EOF) {
          if (nextSymbol == '\\')
            nextSymbol = '\\';
          else if (nextSymbol == 'n')
            nextSymbol = '\n';
          result += nextSymbol;
          source->getNextChar();
          nextSymbol = source->peekNextChar();
          continue;
        }
      }
    } 
    result += source->getNextChar();
    nextSymbol = source->peekNextChar();
  }

  if (source->peekNextChar() == startSymbol) {
    source->getNextChar();
    currentToken.setTokenType(Token::Type::StringLiteral);
    currentToken.setValue(result);
    return currentToken;
  }

  return std::nullopt;
}

std::optional<Token> Lexer::tryNumberToken(int firstSymbol) const {
  if (!isdigit(firstSymbol))
    return std::nullopt;

  int nextSymbol;
  int wholePart = firstSymbol - '0';
  float decimalPart = 0;
  float decimalPartSize = 1.f;
  bool bDotFound = false;
  while (isdigit(nextSymbol = source->peekNextChar()) ||
         (nextSymbol == '.' && !bDotFound)) {
    if (nextSymbol == '.') {
      bDotFound = true;
      source->getNextChar();
    } else {
      if (bDotFound) {
        decimalPart *= 10;
        decimalPartSize *= 10;
        int newDigit = source->getNextChar() - '0';
        if (decimalPart > std::numeric_limits<int>::max() - newDigit)
          throw std::runtime_error("Number too big at " +
                                   source->getCurrentPosition().toString() +
                                   " !");
        decimalPart += newDigit;
      } else {
        wholePart *= 10;
        int newDigit = source->getNextChar() - '0';
        if (wholePart > std::numeric_limits<int>::max() - newDigit)
          throw std::runtime_error("Number too big at " +
                                   source->getCurrentPosition().toString() +
                                   " !");
        wholePart += newDigit;
      }
    }
  }

  decimalPart /= decimalPartSize;

  nextSymbol = source->peekNextChar();
  if (isalpha(nextSymbol))
    return std::nullopt;

  if (bDotFound) {
    currentToken.setTokenType(Token::Type::FloatLiteral);
    currentToken.setValue(wholePart + decimalPart);
    return currentToken;
  } else {
    currentToken.setTokenType(Token::Type::IntLiteral);
    currentToken.setValue(wholePart);
    return currentToken;
  }

  return std::nullopt;
}

std::optional<Token> Lexer::tryOperatorToken(int firstSymbol) const {

  std::string result;
  result += (char)firstSymbol;
  result += source->peekNextChar();
  auto tokenType = tokenMap.findOperator(result);
  if (tokenType != Token::Type::BadType) {
    source->getNextChar();
    currentToken.setTokenType(tokenType);
  } else {
    result.pop_back();
    tokenType = tokenMap.findOperator(result);
    if (tokenType != Token::Type::BadType) {
      currentToken.setTokenType(tokenType);
    } else {
      return std::nullopt;
    }
  }

  return currentToken;
}

std::optional<Token> Lexer::trySymbolToken(int firstSymbol) const {

  std::string result;
  result += (char)firstSymbol;
  auto tokenType = tokenMap.findSymbol(result);
  if (tokenType != Token::Type::BadType)
    currentToken.setTokenType(tokenType);
  else
    return std::nullopt;

  return currentToken;
}

std::optional<Token> Lexer::tryAlphaToken(int firstSymbol) const {
  if (!isalpha(firstSymbol) && firstSymbol != '_')
    return std::nullopt;

  std::string result;
  result += (char)firstSymbol;

  int nextSymbol;
  while (isalnum(nextSymbol = source->peekNextChar()) || nextSymbol == '_')
    result += source->getNextChar();

  auto tokenType = tokenMap.findKeyword(result);
  if (tokenType != Token::Type::BadType)
    currentToken.setTokenType(tokenType);
  else if (result == "true") {
    currentToken.setTokenType(Token::Type::BooleanLiteral);
    currentToken.setValue(true);
  } else if (result == "false") {
    currentToken.setTokenType(Token::Type::BooleanLiteral);
    currentToken.setValue(false);
  } else {
    currentToken.setTokenType(Token::Type::Identifier);
    currentToken.setValue(result);
  }

  return currentToken;
}

void Lexer::consumeWhiteLines() const {
  int nextSymbol = source->peekNextChar();
  while (isspace(nextSymbol) || nextSymbol == EoLSymbol) {
    source->getNextChar();
    nextSymbol = source->peekNextChar();
  }
}
