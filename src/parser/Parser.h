#pragma once
#include "../instructions/Program.h"
#include "../lexer/Token.h"
#include <memory>
#include <optional>
#include <vector>

class Lexer;
class Function;
class Block;
class Instruction;
class Variable;
class Expression;
class Case;

class Parser {
public:
  explicit Parser(std::unique_ptr<Lexer> inLexer);
  Program *getParsedProgram() const;
  Program *parseProgram();

private:
  bool GetAndCheckToken(const std::vector<Token::Type> &tokenTypes);
  bool GetAndCheckTokenNoThrow(const std::vector<Token::Type> &tokenTypes);
  bool PeekAndCheckToken(const std::vector<Token::Type> &tokenTypes);
  bool PeekAndCheckTokenNoThrow(const std::vector<Token::Type> &tokenTypes);
  bool AdvanceIf(const std::vector<Token::Type> &tokenTypes);
  void GetNextToken();
  bool CheckToken(Token::Type tokenType, bool bPeekToken = false) const;
  bool CheckTokenNoThrow(Token::Type tokenType, bool bPeekToken = false) const;
  std::unique_ptr<Function> parseFunction();
  std::unique_ptr<Block> parseBlock();
  std::unique_ptr<Instruction> parseInstruction();
  std::unique_ptr<Instruction> parseDeclaration();
  std::unique_ptr<Instruction> parseFunctionCall(bool bCheckSemiColon = false);
  std::unique_ptr<Instruction> parseAssignment();
  std::unique_ptr<Instruction> parseWhile();
  std::unique_ptr<Instruction> parseReturn();
  std::unique_ptr<Instruction> parseIf();
  std::unique_ptr<Instruction> parseMatch();
  std::unique_ptr<Case> parseCase();
  std::unique_ptr<Expression> parseExpression();

  std::unique_ptr<Expression>
  parseBaseExpression(std::unique_ptr<Expression> lhs);
  std::unique_ptr<Expression>
  parseExpressionAnd(std::unique_ptr<Expression> lhs);
  std::unique_ptr<Expression>
  parseExpressionRelation(std::unique_ptr<Expression> lhs);
  std::unique_ptr<Expression>
  parseExpressionBaseLogic(std::unique_ptr<Expression> lhs);
  std::unique_ptr<Expression>
  parseExpressionMathematical(std::unique_ptr<Expression> lhs);
  std::unique_ptr<Expression>
  parseExpressionMultiplicative(std::unique_ptr<Expression> lhs);
  std::unique_ptr<Expression>
  parseExpressionBaseMathematical(std::unique_ptr<Expression> lhs);
  std::unique_ptr<Expression> parseSimpleExpression();
  std::vector<std::unique_ptr<Expression>> parseArgumentList();
  std::unique_ptr<Variable> parseVariable();
  std::unique_ptr<Variable> parseVariableIdentifier();

  std::unique_ptr<Lexer> lexer;
  std::unique_ptr<Program> parsedProgram;
  std::optional<Token> peekToken;
  Token currentToken;
};
