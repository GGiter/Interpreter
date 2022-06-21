#include "Parser.h"
#include "../instructions/BinaryExpression.h"
#include "../instructions/Block.h"
#include "../instructions/Case.h"
#include "../instructions/Function.h"
#include "../instructions/FunctionCallExpression.h"
#include "../instructions/IfElse.h"
#include "../instructions/InstructionAssigment.h"
#include "../instructions/InstructionDeclarationVariable.h"
#include "../instructions/InstructionFunctionCall.h"
#include "../instructions/InstructionReturn.h"
#include "../instructions/Match.h"
#include "../instructions/ParameterDefinition.h"
#include "../instructions/UnaryExpression.h"
#include "../instructions/Value.h"
#include "../instructions/Variable.h"
#include "../instructions/VariableExpression.h"
#include "../instructions/While.h"
#include "../lexer/Lexer.h"
#include "../lexer/SourcePosition.h"
#include "ParserError.h"
#include <stdexcept>

Parser::Parser(std::unique_ptr<Lexer> inLexer) : lexer(std::move(inLexer)) {}

Program *Parser::getParsedProgram() const { return parsedProgram.get(); }

Program *Parser::parseProgram() {
  if (parsedProgram)
    return parsedProgram.get();

  std::unique_ptr<Program> program = std::make_unique<Program>();

  while (!GetAndCheckTokenNoThrow({Token::Type::Eof}))
    program->addFunction(parseFunction());

  parsedProgram = std::move(program);
  
  return parsedProgram.get();
}

bool Parser::GetAndCheckToken(const std::vector<Token::Type> &tokenTypes) {
  GetNextToken();
  for (auto tokenType : tokenTypes) {
    if (CheckToken(tokenType))
      return true;
  }
  return false;
}

bool Parser::GetAndCheckTokenNoThrow(
    const std::vector<Token::Type> &tokenTypes) {
  GetNextToken();
  for (auto tokenType : tokenTypes) {
    if (CheckTokenNoThrow(tokenType))
      return true;
  }
  return false;
}

bool Parser::PeekAndCheckToken(const std::vector<Token::Type> &tokenTypes) {
  if (!peekToken) {
    peekToken = lexer->getNextToken();
  }
  for (auto tokenType : tokenTypes) {
    if (CheckToken(tokenType, true))
      return true;
  }
  return false;
}

bool Parser::PeekAndCheckTokenNoThrow(
    const std::vector<Token::Type> &tokenTypes) {
  if (!peekToken) {
    peekToken = lexer->getNextToken();
  }
  for (auto tokenType : tokenTypes) {
    if (CheckTokenNoThrow(tokenType, true))
      return true;
  }
  return false;
}

bool Parser::AdvanceIf(const std::vector<Token::Type> &tokenTypes) {
  if (PeekAndCheckTokenNoThrow(tokenTypes)) {
    GetNextToken();
    return true;
  }
  return false;
}

void Parser::GetNextToken() {
  if (!peekToken) {
    currentToken = lexer->getNextToken();
  } else {
    currentToken = *peekToken;
    peekToken = std::nullopt;
  }
}

bool Parser::CheckToken(Token::Type tokenType, bool bPeekToken) const {
  if (CheckTokenNoThrow(tokenType, bPeekToken))
    return true;

  auto tokenToUse = bPeekToken ? peekToken : currentToken;
  throw ParserTokenError("Unexpected token at" +
                             tokenToUse->getStartPosition().toString() + " !",
                         tokenToUse->getTokenType(), tokenType);

  return false;
}

bool Parser::CheckTokenNoThrow(Token::Type tokenType, bool bPeekToken) const {
  if (bPeekToken) {
    if (*peekToken == tokenType)
      return true;
  } else if (currentToken == tokenType)
    return true;
  return false;
}

std::unique_ptr<Function> Parser::parseFunction() {
  CheckToken(Token::Type::Function);
  GetAndCheckToken({Token::Type::Identifier});
  std::unique_ptr<Function> function = std::make_unique<Function>();
  function->setIdentifer(std::get<std::string>(currentToken.getValue()));
  GetAndCheckToken({Token::Type::ParenthesesOpen});
  bool bParameterDefinitionFound = false;
  while (AdvanceIf({Token::Type::Var, Token::Type::Mut})) {
    bParameterDefinitionFound = true;
    bool bIsMutable = false;
    if (currentToken == Token::Type::Mut) {
      bIsMutable = true;
      GetAndCheckToken({Token::Type::Var});
    }

    if (GetAndCheckToken({Token::Type::Identifier})) {
      auto parameter = std::make_unique<ParameterDefinition>(
          std::get<std::string>(currentToken.getValue()), bIsMutable);
      function->addArgument(std::move(parameter));
    }

    if (AdvanceIf({Token::Type::ParenthesesClose})) {
      break;
    }
    GetAndCheckToken({Token::Type::Comma});
  }
  if (!bParameterDefinitionFound)
    GetAndCheckToken({Token::Type::ParenthesesClose});
  function->setBody(parseBlock());
  return function;
}

std::unique_ptr<Block> Parser::parseBlock() {
  GetAndCheckToken({Token::Type::CurlyBracketOpen});

  std::unique_ptr<Block> block = std::make_unique<Block>();
  while (!PeekAndCheckTokenNoThrow({Token::Type::CurlyBracketClose})) {
    block->addInstruction(parseInstruction());
  }
  GetNextToken();
  return block;
}

std::unique_ptr<Instruction> Parser::parseInstruction() {
  if (auto instruction = parseDeclaration()) {
    return instruction;
  } else if (auto instruction = parseFunctionCall(true)) {
    return instruction;
  } else if (auto instruction = parseAssignment()) {
    return instruction;
  } else if (auto instruction = parseWhile()) {
    return instruction;
  } else if (auto instruction = parseReturn()) {
    return instruction;
  } else if (auto instruction = parseMatch()) {
    return instruction;
  } else if (auto instruction = parseIf()) {
    return instruction;
  }
  throw ParserError("No instruction could be parsed at " +
                    currentToken.getStartPosition().toString() + "!");
}

std::unique_ptr<Instruction> Parser::parseDeclaration() {
  if (!AdvanceIf({Token::Type::Mut, Token::Type::Var}))
    return nullptr;
  bool bIsMutable = false;
  if (currentToken == Token::Type::Mut) {
    bIsMutable = true;
    GetAndCheckToken({Token::Type::Var});
  }
  GetAndCheckToken({Token::Type::Identifier});
  std::string name = std::get<std::string>(currentToken.getValue());
  std::unique_ptr<Expression> expression = nullptr;
  if (AdvanceIf({Token::Type::Assign})) {
    expression = std::move(parseExpression());
  }
  GetAndCheckToken({Token::Type::SemiColon});
  return std::make_unique<InstructionDeclarationVariable>(
      name, bIsMutable, std::move(expression));
}

std::unique_ptr<Instruction> Parser::parseFunctionCall(bool bCheckSemiColon) {
  if (!(AdvanceIf({Token::Type::Identifier}) &&
        PeekAndCheckTokenNoThrow({Token::Type::ParenthesesOpen})))
    return nullptr;
  std::string name = std::get<std::string>(currentToken.getValue());
  auto instruction = std::make_unique<InstructionFunctionCall>(name);

  instruction->setArguments(parseArgumentList());
  if (bCheckSemiColon)
    GetAndCheckToken({Token::Type::SemiColon});
  return instruction;
}

std::unique_ptr<Instruction> Parser::parseAssignment() {
  auto variable = parseVariableIdentifier();
  if (!variable)
    return nullptr;
  GetAndCheckToken({Token::Type::Assign});
  auto instruction = std::make_unique<InstructionAssigment>(
      std::move(variable), std::move(parseExpression()));
  GetAndCheckToken({Token::Type::SemiColon});
  return instruction;
}

std::unique_ptr<Instruction> Parser::parseWhile() {
  if (!AdvanceIf({Token::Type::While}))
    return nullptr;
  GetAndCheckToken({Token::Type::ParenthesesOpen});
  std::unique_ptr<Expression> expression = parseExpression();
  GetAndCheckToken({Token::Type::ParenthesesClose});
  std::unique_ptr<Block> block = parseBlock();
  return std::make_unique<While>(std::move(expression), std::move(block));
}

std::unique_ptr<Instruction> Parser::parseReturn() {
  if (!AdvanceIf({Token::Type::Return}))
    return nullptr;
  std::unique_ptr<Expression> expression = nullptr;
  if (!PeekAndCheckTokenNoThrow({Token::Type::SemiColon})) {
    expression = parseExpression();
  }
  GetAndCheckToken({Token::Type::SemiColon});
  return std::make_unique<InstructionReturn>(std::move(expression));
}

std::unique_ptr<Instruction> Parser::parseIf() {
  if (!AdvanceIf({Token::Type::If}))
    return nullptr;
  GetAndCheckToken({Token::Type::ParenthesesOpen});
  std::unique_ptr<Expression> expression = parseExpression();
  GetAndCheckToken({Token::Type::ParenthesesClose});
  std::unique_ptr<Block> ifBlock = parseBlock();
  if (AdvanceIf({Token::Type::Else})) {
    std::unique_ptr<Block> elseBlock = parseBlock();
    return std::make_unique<IfElse>(std::move(expression), std::move(ifBlock),
                                    std::move(elseBlock));
  }
  return std::make_unique<IfElse>(std::move(expression), std::move(ifBlock),
                                  nullptr);
}

std::unique_ptr<Instruction> Parser::parseMatch() {
  if (!AdvanceIf({Token::Type::Match}))
    return nullptr;
  GetAndCheckToken({Token::Type::ParenthesesOpen});
  std::unique_ptr<Expression> expression = parseExpression();
  GetAndCheckToken({Token::Type::ParenthesesClose});
  std::unique_ptr<Match> matchInstruction =
      std::make_unique<Match>(std::move(expression));
  GetAndCheckToken({Token::Type::CurlyBracketOpen});
  while (auto caseInstruction = parseCase()) {
    matchInstruction->addCase(std::move(caseInstruction));
  }

  GetAndCheckToken({Token::Type::CurlyBracketClose});
  return matchInstruction;
}

std::unique_ptr<Case> Parser::parseCase() {
  if (!AdvanceIf({Token::Type::Case}))
    return nullptr;
  std::unique_ptr<Expression> expression = parseExpression();
  GetAndCheckToken({Token::Type::Colon});
  std::unique_ptr<Block> block = parseBlock();
  std::unique_ptr<Case> caseInstruction =
      std::make_unique<Case>(std::move(expression), std::move(block));
  return caseInstruction;
}

std::vector<std::unique_ptr<Expression>> Parser::parseArgumentList() {
  GetAndCheckToken({Token::Type::ParenthesesOpen});
  std::vector<std::unique_ptr<Expression>> expressions;
  while (!PeekAndCheckTokenNoThrow({Token::Type::ParenthesesClose})) {
    expressions.emplace_back(parseExpression());
    if (PeekAndCheckTokenNoThrow({Token::Type::ParenthesesClose})) {
      break;
    }
    GetAndCheckToken({Token::Type::Comma});
  }
  if (!expressions.size())
    GetAndCheckToken({Token::Type::ParenthesesClose});
  else
    GetNextToken(); // consume ParenthesesClose
  return expressions;
}

std::unique_ptr<Expression> Parser::parseExpression() {
  auto expression = std::move(parseExpressionBaseLogic(nullptr));
  expression =
      std::move(parseExpressionBaseMathematical(std::move(expression)));
  expression = std::move(parseBaseExpression(std::move(expression)));
  expression = std::move(parseExpressionAnd(std::move(expression)));
  expression = std::move(parseExpressionRelation(std::move(expression)));
  expression = std::move(parseExpressionMathematical(std::move(expression)));
  expression = std::move(parseExpressionMultiplicative(std::move(expression)));

  if (expression)
    return expression;

  throw ParserExpressionError("Failed to parse expression " +
                              currentToken.getStartPosition().toString() + "!");
}

std::unique_ptr<Expression>
Parser::parseBaseExpression(std::unique_ptr<Expression> lhs) {
  if (!lhs)
    lhs = parseSimpleExpression();
  if (!lhs) {
    return nullptr;
  }
  while (AdvanceIf({Token::Type::LogicalOr})) {
    auto rhs = parseSimpleExpression();
    if (!rhs) {
      throw ParserExpressionError("Error during parsing base expression " +
                                  currentToken.getStartPosition().toString() +
                                  "!");
    }
    auto expression = std::make_unique<BinaryExpression>(
        std::move(lhs), Expression::Operator::LogicalOr, std::move(rhs));
    lhs = std::move(expression);
  }
  return lhs;
}

std::unique_ptr<Expression>
Parser::parseExpressionAnd(std::unique_ptr<Expression> lhs) {
  if (!lhs)
    lhs = parseSimpleExpression();
  if (!lhs) {
    return nullptr;
  }
  while (AdvanceIf({Token::Type::LogicalAnd})) {
    auto rhs = parseSimpleExpression();
    if (!rhs) {
      throw ParserExpressionError("Error during parsing and expression " +
                                  currentToken.getStartPosition().toString() +
                                  "!");
    }
    auto expression = std::make_unique<BinaryExpression>(
        std::move(lhs), Expression::Operator::LogicalAnd, std::move(rhs));
    lhs = std::move(expression);
  }
  return lhs;
}

std::unique_ptr<Expression>
Parser::parseExpressionRelation(std::unique_ptr<Expression> lhs) {
  if (!lhs)
    lhs = parseSimpleExpression();
  if (!lhs) {
    return nullptr;
  }
  while (AdvanceIf({Token::Type::Equal, Token::Type::NotEqual,
                    Token::Type::Less, Token::Type::More,
                    Token::Type::LessEqual, Token::Type::MoreEqual})) {

    Expression::Operator op;
    if (currentToken == Token::Type::Equal)
      op = Expression::Operator::Equal;
    else if (currentToken == Token::Type::NotEqual)
      op = Expression::Operator::NotEqual;
    else if (currentToken == Token::Type::Less)
      op = Expression::Operator::Less;
    else if (currentToken == Token::Type::LessEqual)
      op = Expression::Operator::LessEqual;
    else if (currentToken == Token::Type::More)
      op = Expression::Operator::More;
    else
      op = Expression::Operator::MoreEqual;

    auto rhs = parseSimpleExpression();
    if (!rhs) {
      throw ParserExpressionError("Error during parsing relation expression " +
                                  currentToken.getStartPosition().toString() +
                                  "!");
    }
    auto expression =
        std::make_unique<BinaryExpression>(std::move(lhs), op, std::move(rhs));
    lhs = std::move(expression);
  }
  return lhs;
}

std::unique_ptr<Expression>
Parser::parseExpressionBaseLogic(std::unique_ptr<Expression> lhs) {
  if (AdvanceIf({Token::Type::Negation})) {
    if (lhs)
      throw ParserExpressionError("Error during parsing base logic expression" +
                                  currentToken.getStartPosition().toString() +
                                  "!");

    auto expression = std::make_unique<UnaryExpression>(
        Expression::Operator::Negation, std::move(parseSimpleExpression()));
    lhs = std::move(expression);
  }
  return lhs;
}

std::unique_ptr<Expression>
Parser::parseExpressionMathematical(std::unique_ptr<Expression> lhs) {
  if (!lhs)
    lhs = parseSimpleExpression();
  if (!lhs) {
    return nullptr;
  }
  while (AdvanceIf({Token::Type::Sum, Token::Type::Substraction})) {
    Expression::Operator op = currentToken == Token::Type::Sum
                                  ? Expression::Operator::Sum
                                  : Expression::Operator::Substraction;
    auto rhs = parseSimpleExpression();
    if (!rhs) {
      throw ParserExpressionError(
          "Error during parsing mathematical expression" +
          currentToken.getStartPosition().toString() + "!");
    }
    auto expression =
        std::make_unique<BinaryExpression>(std::move(lhs), op, std::move(rhs));
    lhs = std::move(expression);
  }
  return lhs;
}

std::unique_ptr<Expression>
Parser::parseExpressionMultiplicative(std::unique_ptr<Expression> lhs) {
  if (!lhs)
    lhs = parseSimpleExpression();
  if (!lhs) {
    return nullptr;
  }
  while (AdvanceIf({Token::Type::Multiplication, Token::Type::Division,
                    Token::Type::Modulo})) {

    Expression::Operator op;
    if (currentToken == Token::Type::Multiplication)
      op = Expression::Operator::Multiplication;
    else if (currentToken == Token::Type::Division)
      op = Expression::Operator::Division;
    else
      op = Expression::Operator::Modulo;

    auto rhs = parseSimpleExpression();
    if (!rhs) {
      throw ParserExpressionError(
          "Error during parsing multiplicative expression " +
          currentToken.getStartPosition().toString() + "!");
    }
    auto expression =
        std::make_unique<BinaryExpression>(std::move(lhs), op, std::move(rhs));
    lhs = std::move(expression);
  }
  return lhs;
}

std::unique_ptr<Expression>
Parser::parseExpressionBaseMathematical(std::unique_ptr<Expression> lhs) {
  if (AdvanceIf({Token::Type::Substraction})) {
    if (lhs)
      throw ParserExpressionError(
          "Error during parsing base mathematical expression " +
          currentToken.getStartPosition().toString() + "!");

    auto expression = std::make_unique<UnaryExpression>(
        Expression::Operator::Substraction, std::move(parseSimpleExpression()));
    lhs = std::move(expression);
  }
  return lhs;
}

std::unique_ptr<Expression> Parser::parseSimpleExpression() {
  std::unique_ptr<Expression> expression = nullptr;

  if (AdvanceIf({Token::Type::ParenthesesOpen})) {
    expression = std::move(parseExpression());
    GetAndCheckToken({Token::Type::ParenthesesClose});
  } else if (auto functionCall = parseFunctionCall()) {
    expression =
        std::make_unique<FunctionCallExpression>(std::move(functionCall));
  } else if (auto variable = parseVariableIdentifier()) {
    expression = std::make_unique<VariableExpression>(std::move(variable));
  } else if (auto variable = parseVariable()) {
    expression = std::make_unique<VariableExpression>(std::move(variable));
  } else {
    throw ParserExpressionError("Failed to parse simple expression " +
                                currentToken.getStartPosition().toString() +
                                "!");
  }

  return expression;
}

std::unique_ptr<Variable> Parser::parseVariable() {
  if (!AdvanceIf({Token::Type::StringLiteral, Token::Type::IntLiteral,
                  Token::Type::FloatLiteral, Token::Type::BooleanLiteral}))
    return nullptr;

  return std::make_unique<Variable>(std::make_unique<Value>(
      currentToken.getTokenType(), currentToken.getValue()));
}

std::unique_ptr<Variable> Parser::parseVariableIdentifier() {
  if (!CheckTokenNoThrow(Token::Type::Identifier) &&
      !AdvanceIf({{Token::Type::Identifier}}))
    return nullptr;

  return std::make_unique<Variable>(
      std::get<std::string>(currentToken.getValue()));
}
