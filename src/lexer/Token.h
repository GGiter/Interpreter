#pragma once
#include "SourcePosition.h"
#include <memory>
#include <string>
#include <variant>

constexpr int EoLSymbol = 10;

class Token {
public:
  enum class Type : int {
    Identifier = 0,
    IntLiteral,
    StringLiteral,
    FloatLiteral,
    BooleanLiteral,
    Multiplication,
    Division,
    Modulo,
    Sum,
    Substraction,
    Return,
    Var,
    Mut,
    Function,
    If,
    Else,
    While,
    Match,
    Case,
    CurlyBracketOpen,
    CurlyBracketClose,
    ParenthesesOpen,
    ParenthesesClose,
    Comma,
    Apostrophe, //"'"
    Qoute,   //'"'
    Colon,
    SemiColon,
    Less,
    More,
    LessEqual,
    MoreEqual,
    Equal,
    NotEqual,
    LogicalOr,
    LogicalAnd,
    Negation,
    Assign,
    BackSlash,
    Eof,
    BadType = -1,
  };

  Token() = default;
  explicit Token(const SourcePosition &inStartPosition);
  void setTokenType(Type inType);
  void setValue(std::variant<std::string, float, int, bool> inValue);
  Type getTokenType() const;
  const std::variant<std::string, float, int, bool> &getValue() const;
  const SourcePosition &getStartPosition() const;
  bool operator==(Token::Type InType) const;


private:
  Type type = Type::BadType;
  std::variant<std::string, float, int, bool> value;
  SourcePosition startPosition;
};
