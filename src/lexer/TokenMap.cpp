#include "TokenMap.h"
#include <ranges>

std::map<std::string, Token::Type> TokenMap::keywords = {
    {"return", Token::Type::Return}, {"var", Token::Type::Var},
    {"mut", Token::Type::Mut},       {"if", Token::Type::If},
    {"else", Token::Type::Else},     {"while", Token::Type::While},
    {"match", Token::Type::Match},   {"case", Token::Type::Case},
    {"fn", Token::Type::Function}};

std::map<std::string, Token::Type> TokenMap::operators = {
    {"*", Token::Type::Multiplication}, {"/", Token::Type::Division},
    {"%", Token::Type::Modulo},         {"+", Token::Type::Sum},
    {"-", Token::Type::Substraction},   {"<", Token::Type::Less},
    {">", Token::Type::More},           {"!", Token::Type::Negation},
    {">=", Token::Type::MoreEqual},     {"<=", Token::Type::LessEqual},
    {"==", Token::Type::Equal},         {"||", Token::Type::LogicalOr},
    {"&&", Token::Type::LogicalAnd},    {"!=", Token::Type::NotEqual}};

std::map<std::string, Token::Type> TokenMap::symbols = {
    {"{", Token::Type::CurlyBracketOpen},
    {"}", Token::Type::CurlyBracketClose},
    {"(", Token::Type::ParenthesesOpen},
    {")", Token::Type::ParenthesesClose},
    {",", Token::Type::Comma},
    {"'", Token::Type::Apostrophe},
    {"\"", Token::Type::Qoute},
    {":", Token::Type::Colon},
    {";", Token::Type::SemiColon},
    {"=", Token::Type::Assign},
    {"\\", Token::Type::BackSlash}};

Token::Type TokenMap::findKeyword(const std::string &inString) const {
  const auto iter = keywords.find(inString);
  return iter != keywords.end() ? iter->second : Token::Type::BadType;
}

Token::Type TokenMap::findOperator(const std::string &inString) const {
  const auto iter = operators.find(inString);
  return iter != operators.end() ? iter->second : Token::Type::BadType;
}

Token::Type TokenMap::findSymbol(const std::string &inString) const {
  const auto iter = symbols.find(inString);
  return iter != symbols.end() ? iter->second : Token::Type::BadType;
}

const std::map<std::string, Token::Type> &TokenMap::getKeywords() const {
  return keywords;
}

const std::map<std::string, Token::Type> &TokenMap::getOperators() const {
  return operators;
}

const std::map<std::string, Token::Type> &TokenMap::getSymbols() const {
  return symbols;
}
