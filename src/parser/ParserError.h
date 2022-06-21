#pragma once
#include "../lexer/Token.h"
#include <stdexcept>

class ParserError : public std::runtime_error {
public:
  using _Mybase = std::runtime_error;

  explicit ParserError(const std::string &_Message) : _Mybase(_Message.c_str()) {}

  explicit ParserError(const char *_Message) : _Mybase(_Message) {}
};

class ParserExpressionError : public ParserError {
public:
  using _Mybase = ParserError;

  explicit ParserExpressionError(const std::string &_Message)
      : _Mybase(_Message.c_str()) {}

  explicit ParserExpressionError(const char *_Message) : _Mybase(_Message) {}
};

class ParserTokenError : public ParserError {
public:
  using _Mybase = ParserError;

  explicit ParserTokenError(const std::string &_Message,
                            Token::Type inTokenTypeGot, Token::Type inTokenTypeExpected)
      : _Mybase(_Message.c_str()), TokenTypeGot(inTokenTypeGot), TokenTypeExpected(inTokenTypeExpected) {}

  explicit ParserTokenError(const char *_Message, Token::Type inTokenTypeGot,
                            Token::Type inTokenTypeExpected)
      : _Mybase(_Message), TokenTypeGot(inTokenTypeGot), TokenTypeExpected(inTokenTypeExpected) {}

protected:
  Token::Type TokenTypeGot;
  Token::Type TokenTypeExpected;
};