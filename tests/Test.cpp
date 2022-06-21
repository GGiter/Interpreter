#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Tests

#include <boost/test/unit_test.hpp>

#include "../src/instructions/Block.h"
#include "../src/instructions/Case.h"
#include "../src/instructions/Function.h"
#include "../src/instructions/IfElse.h"
#include "../src/instructions/InstructionAssigment.h"
#include "../src/instructions/InstructionDeclarationVariable.h"
#include "../src/instructions/InstructionFunctionCall.h"
#include "../src/instructions/InstructionReturn.h"
#include "../src/instructions/Match.h"
#include "../src/instructions/Program.h"
#include "../src/instructions/Value.h"
#include "../src/instructions/Variable.h"
#include "../src/instructions/While.h"
#include "../src/interpreter/InterpreterError.h"
#include "../src/interpreter/VisitorInterpreter.h"
#include "../src/interpreter/VisitorInterpreterImpl.h"
#include "../src/lexer/Lexer.h"
#include "../src/lexer/SourceStream.h"
#include "../src/parser/Parser.h"
#include "../src/parser/ParserError.h"

std::unique_ptr<Lexer> configureLexer(const std::string_view &program) {
  std::unique_ptr<Source> source = std::make_unique<SourceStream>(program);

  return std::make_unique<Lexer>(std::move(source));
}

std::unique_ptr<Parser> configureParser(const std::string_view &program) {
  std::unique_ptr<Source> source = std::make_unique<SourceStream>(program);
  std::unique_ptr<Lexer> lexer(std::make_unique<Lexer>(std::move(source)));
  return std::make_unique<Parser>(std::move(lexer));
}

std::unique_ptr<VisitorInterpreter>
configureInterpreter(const std::string_view &program) {
  std::unique_ptr<Source> source = std::make_unique<SourceStream>(program);
  std::unique_ptr<Lexer> lexer(std::make_unique<Lexer>(std::move(source)));
  std::unique_ptr<Parser> parser(std::make_unique<Parser>(std::move(lexer)));
  return std::make_unique<VisitorInterpreterImpl>(std::move(parser));
}

BOOST_AUTO_TEST_SUITE(LEXER)

BOOST_AUTO_TEST_CASE(NumberTest) {
  std::string_view program = "981";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::IntLiteral);
  BOOST_CHECK_EQUAL(std::get<int>(token.getValue()), 981);
}

BOOST_AUTO_TEST_CASE(NumberAssignmentTest) {
  std::string_view program = "var _=981;";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Var);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Identifier);
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "_");

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Assign);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::IntLiteral);
  BOOST_CHECK_EQUAL(std::get<int>(token.getValue()), 981);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::SemiColon);
}

BOOST_AUTO_TEST_CASE(StringAssignmentTest) {
  std::string_view program = "var abc='test';";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Var);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Identifier);
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "abc");

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Assign);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::StringLiteral);
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "test");

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::SemiColon);
}

BOOST_AUTO_TEST_CASE(StringAssignmentTest2) {
  std::string_view program = "var abc=\"test\";";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Var);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Identifier);
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "abc");

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Assign);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::StringLiteral);
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "test");

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::SemiColon);
}

BOOST_AUTO_TEST_CASE(BooleanAssignmentFalseTest) {
  std::string_view program = "var abc=false;";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Var);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Identifier);
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "abc");

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Assign);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(),
                    (int)Token::Type::BooleanLiteral);
  BOOST_CHECK_EQUAL(std::get<bool>(token.getValue()), false);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::SemiColon);
}

BOOST_AUTO_TEST_CASE(BooleanAssignmentTrueTest) {
  std::string_view program = "var abc=true;";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Var);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Identifier);
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "abc");

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Assign);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(),
                    (int)Token::Type::BooleanLiteral);
  BOOST_CHECK_EQUAL(std::get<bool>(token.getValue()), true);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::SemiColon);
}
BOOST_AUTO_TEST_CASE(FloatAssignmentTest) {
  std::string_view program = "var abc=34.42;";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Var);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Identifier);
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "abc");

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Assign);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::FloatLiteral);
  BOOST_TEST(std::get<float>(token.getValue()) == 34.42,
             boost::test_tools::tolerance(0.001));

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::SemiColon);
}

BOOST_AUTO_TEST_CASE(KeywordsTest) {
  TokenMap tokenMap;
  for (const auto &pair : tokenMap.getKeywords()) {
    auto lexer = configureLexer(pair.first);
    auto token = lexer->getNextToken();
    BOOST_CHECK_EQUAL((int)token.getTokenType(),
                      (int)tokenMap.findKeyword(pair.first));

    token = lexer->getNextToken();
    BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Eof);
  }
}

BOOST_AUTO_TEST_CASE(OperatorsTest) {
  TokenMap tokenMap;
  for (const auto &pair : tokenMap.getOperators()) {
    auto lexer = configureLexer(pair.first);
    auto token = lexer->getNextToken();
    BOOST_CHECK_EQUAL((int)token.getTokenType(),
                      (int)tokenMap.findOperator(pair.first));

    token = lexer->getNextToken();
    BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Eof);
  }
}

BOOST_AUTO_TEST_CASE(SymbolsTest) {
  TokenMap tokenMap;
  for (const auto &pair : tokenMap.getSymbols()) {
    auto lexer = configureLexer(pair.first);
    auto token = lexer->getNextToken();
    BOOST_CHECK_EQUAL((int)token.getTokenType(),
                      (int)tokenMap.findSymbol(pair.first));

    token = lexer->getNextToken();
    BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Eof);
  }
}

BOOST_AUTO_TEST_CASE(FailIfTest) {
  std::string_view program = "iff";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_PREDICATE(std::not_equal_to<int>(),
                        ((int)token.getTokenType())((int)Token::Type::If));
}

BOOST_AUTO_TEST_CASE(IdentifierTest) {
  std::string_view program = "abc";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Identifier);

  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "abc");
}

BOOST_AUTO_TEST_CASE(NoAdditionalSymbolsTest) {
  std::string_view program = "if  ";
  auto lexer = configureLexer(program);
  auto token = lexer->getNextToken();

  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::If);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Eof);
}

BOOST_AUTO_TEST_CASE(IfTest) {
  std::string_view program = "if(a>b) { }";
  auto lexer = configureLexer(program);
  auto token = lexer->getNextToken();

  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::If);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(),
                    (int)Token::Type::ParenthesesOpen);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Identifier);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::More);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Identifier);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(),
                    (int)Token::Type::ParenthesesClose);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(),
                    (int)Token::Type::CurlyBracketOpen);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(),
                    (int)Token::Type::CurlyBracketClose);
}

BOOST_AUTO_TEST_CASE(WhileTest) {
  std::string_view program = "while(a<0) { }";
  auto lexer = configureLexer(program);
  auto token = lexer->getNextToken();

  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::While);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(),
                    (int)Token::Type::ParenthesesOpen);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Identifier);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Less);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::IntLiteral);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(),
                    (int)Token::Type::ParenthesesClose);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(),
                    (int)Token::Type::CurlyBracketOpen);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(),
                    (int)Token::Type::CurlyBracketClose);
}

BOOST_AUTO_TEST_CASE(MatchTest) {
  std::string_view program = "match(a) { case a == 1: {} }";
  auto lexer = configureLexer(program);
  auto token = lexer->getNextToken();

  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Match);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(),
                    (int)Token::Type::ParenthesesOpen);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Identifier);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(),
                    (int)Token::Type::ParenthesesClose);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(),
                    (int)Token::Type::CurlyBracketOpen);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Case);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Identifier);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Equal);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::IntLiteral);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Colon);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(),
                    (int)Token::Type::CurlyBracketOpen);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(),
                    (int)Token::Type::CurlyBracketClose);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(),
                    (int)Token::Type::CurlyBracketClose);
}

BOOST_AUTO_TEST_CASE(FailCaseTest) {
  std::string_view program = "Case";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_PREDICATE(std::not_equal_to<int>(),
                        ((int)token.getTokenType())((int)Token::Type::Case));
}

BOOST_AUTO_TEST_CASE(FailCaseTest2) {
  std::string_view program = "Cas:e";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_PREDICATE(std::not_equal_to<int>(),
                        ((int)token.getTokenType())((int)Token::Type::Case));
}

BOOST_AUTO_TEST_CASE(FailVarTest) {
  std::string_view program = "vars";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_PREDICATE(std::not_equal_to<int>(),
                        ((int)token.getTokenType())((int)Token::Type::Var));
}

BOOST_AUTO_TEST_CASE(FindBadTypeInFloatTest) {
  std::string_view program = "981.981.981";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::FloatLiteral);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::BadType);
}

BOOST_AUTO_TEST_CASE(ValueAndIdentifierTest) {
  std::string_view program = "981ABC";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::BadType);
}

BOOST_AUTO_TEST_CASE(MutVarTest) {
  std::string_view program = "mutvar";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_PREDICATE(std::not_equal_to<int>(),
                        ((int)token.getTokenType())((int)Token::Type::Mut));

  BOOST_CHECK_PREDICATE(std::not_equal_to<int>(),
                        ((int)token.getTokenType())((int)Token::Type::Var));
}

BOOST_AUTO_TEST_CASE(TwoFloatsSeperatedByCommaTest) {
  std::string_view program = "981,981";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::IntLiteral);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Comma);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::IntLiteral);
}

BOOST_AUTO_TEST_CASE(IdentifierWithUndercaseTest) {
  std::string_view program = "Test_Test";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Identifier);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Eof);
}

BOOST_AUTO_TEST_CASE(TwoIdentifiersSeperatedByDotTest) {
  std::string_view program = "Test918.Test918";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Identifier);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::BadType);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Identifier);
}

BOOST_AUTO_TEST_CASE(IdentifierWithUndercaseAtStartTest) {
  std::string_view program = "_Test918";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Identifier);

  token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Eof);
}

BOOST_AUTO_TEST_CASE(EofTest) {
  std::string_view program = "";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::Eof);
}

BOOST_AUTO_TEST_CASE(StringWithEscapeSequenceTest) {
  std::string_view program = "\"Strin g\\nSt\\nring\"";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::StringLiteral);
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()),
                    "Strin g\\nSt\\nring");
}

BOOST_AUTO_TEST_CASE(StringWithNoEndSymbolTest) {
  std::string_view program = R"("Strin gString\)";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_PREDICATE(
      std::not_equal_to<int>(),
      ((int)token.getTokenType())((int)Token::Type::StringLiteral));
}

BOOST_AUTO_TEST_CASE(StringWithNoEndSymbolTest2) {
  std::string_view program = R"('Strin gString\)";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_PREDICATE(
      std::not_equal_to<int>(),
      ((int)token.getTokenType())((int)Token::Type::StringLiteral));
}

BOOST_AUTO_TEST_CASE(StringWithSpecialSymbolsTest) {
  std::string_view program = R"("¥█©")";
  auto lexer = configureLexer(program);

  auto token = lexer->getNextToken();
  BOOST_CHECK_EQUAL((int)token.getTokenType(), (int)Token::Type::StringLiteral);
  BOOST_CHECK_EQUAL(std::get<std::string>(token.getValue()), "¥█©");
}

BOOST_AUTO_TEST_CASE(NumberOverflowTest) {
  std::string_view program = "2147483649";
  auto lexer = configureLexer(program);
  BOOST_CHECK_THROW(lexer->getNextToken(), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(PARSER)

BOOST_AUTO_TEST_CASE(AssignIntValueTest) {
  std::string program = "fn main(){var b; b = 5;}";
  auto parser = configureParser(program);

  BOOST_CHECK_NO_THROW(parser->parseProgram());
  BOOST_CHECK_EQUAL(parser->getParsedProgram()->toString(),
                    "fn main(){var b;b=5;}");

  BOOST_CHECK_NO_THROW(parser->getParsedProgram()->getMain()->getBlock());
  auto block = parser->getParsedProgram()->getMain()->getBlock();

  BOOST_CHECK_EQUAL(parser->getParsedProgram()
                        ->getMain()
                        ->getBlock()
                        ->getInstructions()
                        .size(),
                    2);

  InstructionDeclarationVariable *declaration =
      static_cast<InstructionDeclarationVariable *>(
          block->getInstructions()[0].get());
  BOOST_CHECK_NE(declaration, nullptr);

  BOOST_CHECK_EQUAL(declaration->isMutable(), false);
  BOOST_CHECK_EQUAL(declaration->getIdentifier(), "b");

  InstructionAssigment *assignment =
      static_cast<InstructionAssigment *>(block->getInstructions()[1].get());
  BOOST_CHECK_NE(assignment, nullptr);

  BOOST_CHECK_EQUAL(*assignment->getVariable()->getName(), "b");
  BOOST_CHECK_EQUAL(assignment->getExpression()->toString(), "5");
}

BOOST_AUTO_TEST_CASE(FunctionWithParamtersTest) {
  std::string program = "fn main(var a, var b){}";
  auto parser = configureParser(program);

  BOOST_CHECK_NO_THROW(parser->parseProgram());
  BOOST_CHECK_EQUAL(parser->getParsedProgram()->toString(),
                    "fn main(var a,var b){}");

  BOOST_CHECK_NO_THROW(parser->getParsedProgram()->getMain());
  auto function = parser->getParsedProgram()->getMain();

  BOOST_CHECK_EQUAL(function->getArguments().size(), 2);

  BOOST_CHECK_EQUAL(function->getArguments()[0]->getName(), "a");
  BOOST_CHECK_EQUAL(function->getArguments()[0]->isMutable(), false);

  BOOST_CHECK_EQUAL(function->getArguments()[1]->getName(), "b");
  BOOST_CHECK_EQUAL(function->getArguments()[1]->isMutable(), false);
}

BOOST_AUTO_TEST_CASE(AssignNegativeIntValueTest) {
  std::string program = "fn main(){var b; b = -5;}";
  auto parser = configureParser(program);

  BOOST_CHECK_NO_THROW(parser->parseProgram());
  BOOST_CHECK_EQUAL(parser->getParsedProgram()->toString(),
                    "fn main(){var b;b=-5;}");

  BOOST_CHECK_NO_THROW(parser->getParsedProgram()->getMain()->getBlock());
  auto block = parser->getParsedProgram()->getMain()->getBlock();
  BOOST_CHECK_EQUAL(parser->getParsedProgram()
                        ->getMain()
                        ->getBlock()
                        ->getInstructions()
                        .size(),
                    2);

  InstructionDeclarationVariable *declaration =
      static_cast<InstructionDeclarationVariable *>(
          block->getInstructions()[0].get());
  BOOST_CHECK_NE(declaration, nullptr);

  BOOST_CHECK_EQUAL(declaration->isMutable(), false);
  BOOST_CHECK_EQUAL(declaration->getIdentifier(), "b");

  InstructionAssigment *assignment =
      static_cast<InstructionAssigment *>(block->getInstructions()[1].get());
  BOOST_CHECK_NE(assignment, nullptr);

  BOOST_CHECK_EQUAL(*assignment->getVariable()->getName(), "b");
  BOOST_CHECK_EQUAL(assignment->getExpression()->toString(), "-5");
}

BOOST_AUTO_TEST_CASE(AssingValueFromMultiplicativeExpressionTest) {
  std::string program = "fn main(){var b = -5 * 5;}";
  auto parser = configureParser(program);

  BOOST_CHECK_NO_THROW(parser->parseProgram());
  BOOST_CHECK_EQUAL(parser->getParsedProgram()->toString(),
                    "fn main(){var b=-5*5;}");

  BOOST_CHECK_NO_THROW(
      parser->getParsedProgram()->getMain()->getBlock()->getInstructions());
  auto block = parser->getParsedProgram()->getMain()->getBlock();
  BOOST_CHECK_EQUAL(parser->getParsedProgram()
                        ->getMain()
                        ->getBlock()
                        ->getInstructions()
                        .size(),
                    1);

  InstructionDeclarationVariable *declaration =
      static_cast<InstructionDeclarationVariable *>(
          block->getInstructions()[0].get());
  BOOST_CHECK_NE(declaration, nullptr);

  BOOST_CHECK_EQUAL(declaration->isMutable(), false);
  BOOST_CHECK_EQUAL(declaration->getIdentifier(), "b");
  BOOST_CHECK_EQUAL(declaration->getExpression()->toString(), "-5*5");
}

BOOST_AUTO_TEST_CASE(AssingValueFromFunctionCallTest) {
  std::string program = "fn main(){mut var b = test(1*2);}";
  auto parser = configureParser(program);

  BOOST_CHECK_NO_THROW(parser->parseProgram());
  BOOST_CHECK_EQUAL(parser->getParsedProgram()->toString(),
                    "fn main(){mut var b=test(1*2);;}");

  BOOST_CHECK_NO_THROW(
      parser->getParsedProgram()->getMain()->getBlock()->getInstructions());
  auto block = parser->getParsedProgram()->getMain()->getBlock();
  BOOST_CHECK_EQUAL(parser->getParsedProgram()
                        ->getMain()
                        ->getBlock()
                        ->getInstructions()
                        .size(),
                    1);

  InstructionDeclarationVariable *declaration =
      static_cast<InstructionDeclarationVariable *>(
          block->getInstructions()[0].get());
  BOOST_CHECK_NE(declaration, nullptr);

  BOOST_CHECK_EQUAL(declaration->isMutable(), true);
  BOOST_CHECK_EQUAL(declaration->getIdentifier(), "b");
  BOOST_CHECK_EQUAL(declaration->getExpression()->toString(), "test(1*2);");
}

BOOST_AUTO_TEST_CASE(ReturnFromFunctionIntValueTest) {
  std::string program = "fn main(){return 5*5;}";
  auto parser = configureParser(program);

  BOOST_CHECK_NO_THROW(parser->parseProgram());
  BOOST_CHECK_EQUAL(parser->getParsedProgram()->toString(),
                    "fn main(){return 5*5;}");

  BOOST_CHECK_NO_THROW(
      parser->getParsedProgram()->getMain()->getBlock()->getInstructions());
  auto block = parser->getParsedProgram()->getMain()->getBlock();
  BOOST_CHECK_EQUAL(parser->getParsedProgram()
                        ->getMain()
                        ->getBlock()
                        ->getInstructions()
                        .size(),
                    1);

  InstructionReturn *returnInstruction =
      static_cast<InstructionReturn *>(block->getInstructions()[0].get());
  BOOST_CHECK_NE(returnInstruction, nullptr);

  BOOST_CHECK_EQUAL(returnInstruction->getExpression()->toString(), "5*5");
}

BOOST_AUTO_TEST_CASE(ReturnNotEqualTest) {
  std::string program = "fn main(){return 5!=5;}";
  auto parser = configureParser(program);

  BOOST_CHECK_NO_THROW(parser->parseProgram());
  BOOST_CHECK_EQUAL(parser->getParsedProgram()->toString(),
                    "fn main(){return 5!=5;}");

  BOOST_CHECK_NO_THROW(
      parser->getParsedProgram()->getMain()->getBlock()->getInstructions());
  auto block = parser->getParsedProgram()->getMain()->getBlock();
  BOOST_CHECK_EQUAL(parser->getParsedProgram()
                        ->getMain()
                        ->getBlock()
                        ->getInstructions()
                        .size(),
                    1);

  InstructionReturn *returnInstruction =
      static_cast<InstructionReturn *>(block->getInstructions()[0].get());
  BOOST_CHECK_NE(returnInstruction, nullptr);

  BOOST_CHECK_EQUAL(returnInstruction->getExpression()->toString(), "5!=5");
}

BOOST_AUTO_TEST_CASE(FunctionCallTest) {
  std::string program = "fn main(){print(\"abc\");}";
  auto parser = configureParser(program);

  BOOST_CHECK_NO_THROW(parser->parseProgram());
  BOOST_CHECK_EQUAL(parser->getParsedProgram()->toString(),
                    "fn main(){print(\"abc\");}");

  BOOST_CHECK_NO_THROW(
      parser->getParsedProgram()->getMain()->getBlock()->getInstructions());
  auto block = parser->getParsedProgram()->getMain()->getBlock();
  BOOST_CHECK_EQUAL(parser->getParsedProgram()
                        ->getMain()
                        ->getBlock()
                        ->getInstructions()
                        .size(),
                    1);

  InstructionFunctionCall *functionCall =
      static_cast<InstructionFunctionCall *>(block->getInstructions()[0].get());
  BOOST_CHECK_NE(functionCall, nullptr);

  BOOST_CHECK_EQUAL(functionCall->getFunctionName(), "print");
  BOOST_CHECK_EQUAL(functionCall->getExpressions().size(), 1);
  BOOST_CHECK_EQUAL(functionCall->getExpressions()[0]->toString(), "\"abc\"");
}

BOOST_AUTO_TEST_CASE(NegationExpressionTest) {
  std::string program = "fn main(){return !(5>7);}";
  auto parser = configureParser(program);

  BOOST_CHECK_NO_THROW(parser->parseProgram());
  BOOST_CHECK_EQUAL(parser->getParsedProgram()->toString(),
                    "fn main(){return !5>7;}");

  BOOST_CHECK_NO_THROW(
      parser->getParsedProgram()->getMain()->getBlock()->getInstructions());
  auto block = parser->getParsedProgram()->getMain()->getBlock();
  BOOST_CHECK_EQUAL(parser->getParsedProgram()
                        ->getMain()
                        ->getBlock()
                        ->getInstructions()
                        .size(),
                    1);

  InstructionReturn *returnInstruction =
      static_cast<InstructionReturn *>(block->getInstructions()[0].get());
  BOOST_CHECK_NE(returnInstruction, nullptr);

  BOOST_CHECK_EQUAL(returnInstruction->getExpression()->toString(), "!5>7");
}

BOOST_AUTO_TEST_CASE(IfWithLessExpressionTest) {
  std::string program = "fn main(){if(b < c){}else{}}";
  auto parser = configureParser(program);

  BOOST_CHECK_NO_THROW(parser->parseProgram());
  BOOST_CHECK_EQUAL(parser->getParsedProgram()->toString(),
                    "fn main(){if(b<c){}else{}}");

  BOOST_CHECK_NO_THROW(
      parser->getParsedProgram()->getMain()->getBlock()->getInstructions());
  auto block = parser->getParsedProgram()->getMain()->getBlock();
  BOOST_CHECK_EQUAL(parser->getParsedProgram()
                        ->getMain()
                        ->getBlock()
                        ->getInstructions()
                        .size(),
                    1);

  IfElse *ifElse = static_cast<IfElse *>(block->getInstructions()[0].get());
  BOOST_CHECK_NE(ifElse, nullptr);

  BOOST_CHECK_EQUAL(ifElse->getExpression()->toString(), "b<c");
  BOOST_CHECK_NE(ifElse->getBlockElse(), nullptr);
}

BOOST_AUTO_TEST_CASE(WhileTest) {
  std::string program = "fn main(){while(a > 10){}}";
  auto parser = configureParser(program);

  BOOST_CHECK_NO_THROW(parser->parseProgram());
  BOOST_CHECK_EQUAL(parser->getParsedProgram()->toString(),
                    "fn main(){while(a>10){}}");

  BOOST_CHECK_NO_THROW(
      parser->getParsedProgram()->getMain()->getBlock()->getInstructions());
  auto block = parser->getParsedProgram()->getMain()->getBlock();

  BOOST_CHECK_EQUAL(parser->getParsedProgram()
                        ->getMain()
                        ->getBlock()
                        ->getInstructions()
                        .size(),
                    1);

  While *whileInstruction = whileInstruction =
      static_cast<While *>(block->getInstructions()[0].get());
  BOOST_CHECK_NE(whileInstruction, nullptr);

  BOOST_CHECK_EQUAL(whileInstruction->getBody()->getInstructions().size(), 0);
  BOOST_CHECK_EQUAL(whileInstruction->getExpression()->toString(), "a>10");
}

BOOST_AUTO_TEST_CASE(MatchTest) {
  std::string program = "fn main(){match(a){ case a > b:{} case a < b:{}}}";
  auto parser = configureParser(program);

  BOOST_CHECK_NO_THROW(parser->parseProgram());
  BOOST_CHECK_EQUAL(parser->getParsedProgram()->toString(),
                    "fn main(){match(a){case a>b:{}case a<b:{}}}");
  BOOST_CHECK_NO_THROW(
      parser->getParsedProgram()->getMain()->getBlock()->getInstructions());
  auto block = parser->getParsedProgram()->getMain()->getBlock();

  BOOST_CHECK_EQUAL(parser->getParsedProgram()
                        ->getMain()
                        ->getBlock()
                        ->getInstructions()
                        .size(),
                    1);

  Match *match = static_cast<Match *>(block->getInstructions()[0].get());
  BOOST_CHECK_NE(match, nullptr);

  BOOST_CHECK_EQUAL(match->getCases().size(), 2);
  BOOST_CHECK_EQUAL(match->getCases()[0]->getExpression()->toString(), "a>b");

  BOOST_CHECK_EQUAL(match->getCases()[0]->getBlock()->getInstructions().size(),
                    0);

  BOOST_CHECK_EQUAL(match->getCases()[1]->getExpression()->toString(), "a<b");

  BOOST_CHECK_EQUAL(match->getCases()[1]->getBlock()->getInstructions().size(),
                    0);
}

BOOST_AUTO_TEST_CASE(EmptyReturnTest) {
  std::string program = "fn main() { return; }";
  auto parser = configureParser(program);

  BOOST_CHECK_NO_THROW(parser->parseProgram());
  BOOST_CHECK_EQUAL(parser->getParsedProgram()->toString(),
                    "fn main(){return;}");

  BOOST_CHECK_NO_THROW(
      parser->getParsedProgram()->getMain()->getBlock()->getInstructions());
  auto block = parser->getParsedProgram()->getMain()->getBlock();

  BOOST_CHECK_EQUAL(parser->getParsedProgram()
                        ->getMain()
                        ->getBlock()
                        ->getInstructions()
                        .size(),
                    1);

  InstructionReturn *returnInstruction =
      static_cast<InstructionReturn *>(block->getInstructions()[0].get());
  BOOST_CHECK_NE(returnInstruction, nullptr);
}

BOOST_AUTO_TEST_CASE(TwoFunctionsTest) {
  std::string program = "fn fib() {return 0;} fn main(){}";
  auto parser = configureParser(program);

  BOOST_CHECK_NO_THROW(parser->parseProgram());
  BOOST_CHECK_EQUAL(parser->getParsedProgram()->toString(),
                    "fn fib(){return 0;}fn main(){}");

  BOOST_CHECK_EQUAL(parser->getParsedProgram()->getFunctions().size(), 2);
  BOOST_CHECK_EQUAL(
      parser->getParsedProgram()->getFunctions()[0]->getIdentifier(), "fib");
  BOOST_CHECK_EQUAL(
      parser->getParsedProgram()->getFunctions()[0]->getArguments().size(), 0);
  BOOST_CHECK_EQUAL(
      parser->getParsedProgram()->getFunctions()[1]->getArguments().size(), 0);
  BOOST_CHECK_EQUAL(
      parser->getParsedProgram()->getFunctions()[1]->getIdentifier(), "main");
}

BOOST_AUTO_TEST_CASE(MatchWithReturnStatementTest) {
  std::string program = "fn main(){ match(a>b){ case 1:{ return 1;}}}";
  auto parser = configureParser(program);

  BOOST_CHECK_NO_THROW(parser->parseProgram());
  BOOST_CHECK_EQUAL(parser->getParsedProgram()->toString(),
                    "fn main(){match(a>b){case 1:{return 1;}}}");

  BOOST_CHECK_NO_THROW(
      parser->getParsedProgram()->getMain()->getBlock()->getInstructions());
  auto block = parser->getParsedProgram()->getMain()->getBlock();

  BOOST_CHECK_EQUAL(parser->getParsedProgram()
                        ->getMain()
                        ->getBlock()
                        ->getInstructions()
                        .size(),
                    1);

  Match *match = static_cast<Match *>(block->getInstructions()[0].get());
  BOOST_CHECK_NE(match, nullptr);

  BOOST_CHECK_EQUAL(match->getCases().size(), 1);
  BOOST_CHECK_EQUAL(match->getCases()[0]->getExpression()->toString(), "1");

  BOOST_CHECK_EQUAL(match->getCases()[0]->getBlock()->getInstructions().size(),
                    1);

  auto returnInstruction = static_cast<InstructionReturn *>(
      match->getCases()[0]->getBlock()->getInstructions()[0].get());
  BOOST_CHECK_NE(returnInstruction, nullptr);
  BOOST_CHECK_EQUAL(returnInstruction->getExpression()->toString(), "1");
}

BOOST_AUTO_TEST_CASE(StringAssigmentTest) {
  std::string program = "fn main(){ var a=\"Test\";}";
  auto parser = configureParser(program);

  BOOST_CHECK_NO_THROW(parser->parseProgram());
  BOOST_CHECK_EQUAL(parser->getParsedProgram()->toString(),
                    "fn main(){var a=\"Test\";}");

  BOOST_CHECK_NO_THROW(
      parser->getParsedProgram()->getMain()->getBlock()->getInstructions());
  auto block = parser->getParsedProgram()->getMain()->getBlock();

  BOOST_CHECK_EQUAL(parser->getParsedProgram()
                        ->getMain()
                        ->getBlock()
                        ->getInstructions()
                        .size(),
                    1);

  InstructionDeclarationVariable *declaration =
      static_cast<InstructionDeclarationVariable *>(
          block->getInstructions()[0].get());
  BOOST_CHECK_NE(declaration, nullptr);

  BOOST_CHECK_EQUAL(declaration->isMutable(), false);
  BOOST_CHECK_EQUAL(declaration->getIdentifier(), "a");
  BOOST_CHECK_EQUAL(declaration->getExpression()->toString(), "\"Test\"");
}

BOOST_AUTO_TEST_CASE(BooleanAssigmentTest) {
  std::string program = "fn main(){ var a=true;}";
  auto parser = configureParser(program);

  BOOST_CHECK_NO_THROW(parser->parseProgram());
  BOOST_CHECK_EQUAL(parser->getParsedProgram()->toString(),
                    "fn main(){var a=true;}");

  BOOST_CHECK_NO_THROW(
      parser->getParsedProgram()->getMain()->getBlock()->getInstructions());
  auto block = parser->getParsedProgram()->getMain()->getBlock();

  BOOST_CHECK_EQUAL(parser->getParsedProgram()
                        ->getMain()
                        ->getBlock()
                        ->getInstructions()
                        .size(),
                    1);

  InstructionDeclarationVariable *declaration =
      static_cast<InstructionDeclarationVariable *>(
          block->getInstructions()[0].get());
  BOOST_CHECK_NE(declaration, nullptr);

  BOOST_CHECK_EQUAL(declaration->isMutable(), false);
  BOOST_CHECK_EQUAL(declaration->getIdentifier(), "a");
  BOOST_CHECK_EQUAL(declaration->getExpression()->toString(), "true");
}

BOOST_AUTO_TEST_CASE(SimpleIncorrectExampleTest) {
  std::string program = "fn main(){a}";
  auto parser = configureParser(program);

  BOOST_CHECK_THROW(parser->parseProgram(), ParserError);
}

BOOST_AUTO_TEST_CASE(NoSemiColonAfterReturnTest) {
  std::string program = "fn main(){ return }";
  auto parser = configureParser(program);

  BOOST_CHECK_THROW(parser->parseProgram(), ParserError);
}

BOOST_AUTO_TEST_CASE(IncorrectExampleWithMutVarTest) {
  std::string program = "fn main(){ var mut a; }";
  auto parser = configureParser(program);

  BOOST_CHECK_THROW(parser->parseProgram(), ParserError);
}

BOOST_AUTO_TEST_CASE(MatchWithoutCaseTest) {
  std::string program = "fn main(){match(a){ if(a > b) {}}}";
  auto parser = configureParser(program);

  BOOST_CHECK_THROW(parser->parseProgram(), ParserError);
}

BOOST_AUTO_TEST_CASE(IfWithoutBracketsTest) {
  std::string program = "fn main(){if(a > b)}";
  auto parser = configureParser(program);

  BOOST_CHECK_THROW(parser->parseProgram(), ParserError);
}

BOOST_AUTO_TEST_CASE(EmptyIfTest) {
  std::string program = "fn main(){if(){}}";
  auto parser = configureParser(program);

  BOOST_CHECK_THROW(parser->parseProgram(), ParserError);
}

BOOST_AUTO_TEST_CASE(InvalidTokenTest) {
  std::string program = "fn main(){int a;}";
  auto parser = configureParser(program);

  BOOST_CHECK_THROW(parser->parseProgram(), ParserError);
}

BOOST_AUTO_TEST_CASE(InvalidFunctionTest1) {
  std::string program = "fn main(int a,){int a;}";
  auto parser = configureParser(program);

  BOOST_CHECK_THROW(parser->parseProgram(), ParserError);
}

BOOST_AUTO_TEST_CASE(InvalidFunctionTest2) {
  std::string program = "fn main();";
  auto parser = configureParser(program);

  BOOST_CHECK_THROW(parser->parseProgram(), ParserError);
}

BOOST_AUTO_TEST_CASE(InvalidFunctionTest3) {
  std::string program = "fn main( {}";
  auto parser = configureParser(program);

  BOOST_CHECK_THROW(parser->parseProgram(), ParserError);
}

BOOST_AUTO_TEST_CASE(InvalidExpressionTest) {
  std::string program = "fn main() { return >b; }";
  auto parser = configureParser(program);

  BOOST_CHECK_THROW(parser->parseProgram(), ParserExpressionError);
}

BOOST_AUTO_TEST_CASE(InvalidExpressionTest2) {
  std::string program = "fn main() { return b>; }";
  auto parser = configureParser(program);

  BOOST_CHECK_THROW(parser->parseProgram(), ParserExpressionError);
}

BOOST_AUTO_TEST_CASE(InvalidExpressionTest3) {
  std::string program = "fn main() { return >; }";
  auto parser = configureParser(program);

  BOOST_CHECK_THROW(parser->parseProgram(), ParserExpressionError);
}

BOOST_AUTO_TEST_CASE(ExpressionInFunctionDefinitionTest) {
  std::string program = "fn main(var a*b) {}";
  auto parser = configureParser(program);

  BOOST_CHECK_THROW(parser->parseProgram(), ParserError);
}

BOOST_AUTO_TEST_CASE(OrStatementFail) {
  std::string program = "fn main() { return 5 > 6 || 5 < 6; }";
  auto parser = configureParser(program);

  BOOST_CHECK_THROW(parser->parseProgram(), ParserError);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(INTERPRETER)

BOOST_AUTO_TEST_CASE(EmptyReturnTest) {
  std::string program = "fn main() { return; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(interpreter->execute().has_value(), false);
}

BOOST_AUTO_TEST_CASE(IntReturnTest) {
  std::string program = "fn main() { return 1; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<int>(interpreter->execute()->first), 1);
}

BOOST_AUTO_TEST_CASE(AdditionTest) {
  std::string program = "fn main() { var a = 5; var b = 6; return a + b; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<int>(interpreter->execute()->first), 11);
}

BOOST_AUTO_TEST_CASE(AssignValueTwiceTest) {
  std::string program = "fn main() { mut var a = 5; a = 5.14; return a; }";
  auto interpreter = configureInterpreter(program);

  BOOST_TEST(std::get<float>(interpreter->execute()->first) == 5.14,
             boost::test_tools::tolerance(0.001));
}

BOOST_AUTO_TEST_CASE(AdditionTwoTypesTest) {
  std::string program =
      "fn main() { var a = 5.0; var b = 6; return int(a) + b; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<int>(interpreter->execute()->first), 11);
}

BOOST_AUTO_TEST_CASE(ScopeTest) {
  std::string program = "fn test(var a) { var b = 6; return a + b; }fn main() { var b = 5; return test(b); }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<int>(interpreter->execute()->first), 11);
}

BOOST_AUTO_TEST_CASE(ExpressionTest) {
  std::string program = "fn main() { return 5 * 3 / 10; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<int>(interpreter->execute()->first), 1);
}

BOOST_AUTO_TEST_CASE(ExpressionTest2) {
  std::string program = "fn main() { return 5.0 * 3.0 / 10.0; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<float>(interpreter->execute()->first), 1.5);
}

BOOST_AUTO_TEST_CASE(ExpressionTest3) {
  std::string program = "fn main() { return !(( 5<6 )||( 6<7 )); }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<bool>(interpreter->execute()->first), false);
}

BOOST_AUTO_TEST_CASE(ModuloTest) {
  std::string program = "fn main() { return 5 % 3; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<int>(interpreter->execute()->first), 2);
}

BOOST_AUTO_TEST_CASE(BoolToBoolTest) {
  std::string program = "fn main() { return bool(true); }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<bool>(interpreter->execute()->first), true);
}

BOOST_AUTO_TEST_CASE(NegationTest) {
  std::string program = "fn main() { return !false; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<bool>(interpreter->execute()->first), true);
}

BOOST_AUTO_TEST_CASE(StringToBoolTest) {
  std::string program = "fn main() { return bool(\"true\"); }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<bool>(interpreter->execute()->first), true);
}

BOOST_AUTO_TEST_CASE(IntToBoolTest) {
  std::string program = "fn main() { return bool(2); }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<bool>(interpreter->execute()->first), true);
}

BOOST_AUTO_TEST_CASE(IntToBoolTest2) {
  std::string program = "fn main() { return bool(-2); }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<bool>(interpreter->execute()->first), false);
}

BOOST_AUTO_TEST_CASE(FloatToBoolTest) {
  std::string program = "fn main() { return bool(2.5); }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<bool>(interpreter->execute()->first), true);
}

BOOST_AUTO_TEST_CASE(FloatToBoolTest2) {
  std::string program = "fn main() { return bool(-2.5); }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<bool>(interpreter->execute()->first), false);
}

BOOST_AUTO_TEST_CASE(StringToFloatTest) {
  std::string program = "fn main() { return float(\"3.0\"); }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<float>(interpreter->execute()->first), 3.0);
}

BOOST_AUTO_TEST_CASE(FloatToStringTest) {
  std::string program = "fn main() { return string(3.0); }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<std::string>(interpreter->execute()->first)[0],
                    '3');
  BOOST_CHECK_EQUAL(std::get<std::string>(interpreter->execute()->first)[1],
                    '.');
  BOOST_CHECK_EQUAL(std::get<std::string>(interpreter->execute()->first)[2],
                    '0');
}

BOOST_AUTO_TEST_CASE(BoolToStringTest) {
  std::string program = "fn main() { return string(true); }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<std::string>(interpreter->execute()->first),
                    "true");
}

BOOST_AUTO_TEST_CASE(ModuloTest2) {
  std::string program = "fn main() { return 5.0 % 3.0; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<float>(interpreter->execute()->first), 2.0);
}

BOOST_AUTO_TEST_CASE(LessEqualTest) {
  std::string program = "fn main() { var a = 2; var b = 3; return a <= b; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<bool>(interpreter->execute()->first), true);
}

BOOST_AUTO_TEST_CASE(TwoFunctionsTest) {
  std::string program = "fn add(var a, var b) { return (a + b); } fn main() { "
                        "var c = add(5, 6); return c; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<int>(interpreter->execute()->first), 11);
}

BOOST_AUTO_TEST_CASE(TwoFunctionsTest2) {
  std::string program = "fn add(var a, var b) { return (a + b); } fn main() { "
                        "var d = 5; var c = add(5, 6); return c + d; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<int>(interpreter->execute()->first), 16);
}

BOOST_AUTO_TEST_CASE(FibTest) {
  std::string program =
      "fn fib(var n) { if (n <= 1) {return n;} else { return fib(n-1) + "
      "fib(n-2);}} fn main() {var n = fib(3); return n; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<int>(interpreter->execute()->first), 2);
}

BOOST_AUTO_TEST_CASE(WhileTest) {
  std::string program = "fn main(){ mut var a = 10; mut var c = 0; while(a > "
                        "0){ c = c + 1; a = a - 1;} return c;}";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<int>(interpreter->execute()->first), 10);
}

BOOST_AUTO_TEST_CASE(TypeChangeTest) {
  std::string program =
      "fn main() { mut var a = 5.0; a = \"dynamiczny\"; return a; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<std::string>(interpreter->execute()->first),
                    "dynamiczny");
}

BOOST_AUTO_TEST_CASE(TypeChangeTest2) {
  std::string program =
      "fn main() { mut var a = 5.0; var b = 6; a = int(a); return a + b; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<int>(interpreter->execute()->first), 11);
}


BOOST_AUTO_TEST_CASE(MatchTest) {
  std::string program = "fn main() { mut var a = \"dynamiczny\"; match(a){ "
                        "case (_ == \"quit\") || (_ == \"exit\"): { a = "
                        "\"quit\"; } case _: { a = \"unknown\"; }} return a; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<std::string>(interpreter->execute()->first),
                    "unknown");
}

BOOST_AUTO_TEST_CASE(MatchTest2) {
  std::string program = "fn main() { mut var a = \"exit\"; match(a){ "
                        "case (_ == \"quit\") || (_ == \"exit\"): { a = "
                        "\"quit\"; } case _: { a = \"unknown\"; }} return a; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<std::string>(interpreter->execute()->first),
                    "quit");
}

BOOST_AUTO_TEST_CASE(MatchTest3) {
  std::string program = "fn main() { mut var a = 5; var b = 6; match(a+b){ "
                        "case a < b: { a = 6; } case _: { a = \"unknown\"; }} return a; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<int>(interpreter->execute()->first), 6);
}

BOOST_AUTO_TEST_CASE(NoInitializationTest) {
  std::string program = "fn main() { var a; return a; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<int>(interpreter->execute()->first), 0);
}

BOOST_AUTO_TEST_CASE(ModuloFloatTest) {
  std::string program = "fn main() { return 5.0%1.0; }";
  auto interpreter = configureInterpreter(program);

 BOOST_TEST(std::get<float>(interpreter->execute()->first) == 0.0,
            boost::test_tools::tolerance(0.001));
}

BOOST_AUTO_TEST_CASE(DefaultValueTest) {
  std::string program = "fn main() { var a; return a + 5; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<int>(interpreter->execute()->first), 5);
}

BOOST_AUTO_TEST_CASE(StringAdditionTest) {
  std::string program = "fn main() { var a = 'abc'; return a + 'abc'; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_EQUAL(std::get<std::string>(interpreter->execute()->first), "abcabc");
}

BOOST_AUTO_TEST_CASE(StringSubstractionFailTest) {
  std::string program = "fn main() { var a = 'abc'; return a - 'abc'; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_THROW(interpreter->execute(), InterpreterError);
}

BOOST_AUTO_TEST_CASE(DefaultValueFailTest) {
  std::string program = "fn main() { var a; return a + 'abc'; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_THROW(interpreter->execute(), InterpreterError);
}

BOOST_AUTO_TEST_CASE(ModuloBy0FailTest) {
  std::string program = "fn main() { return 5%0; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_THROW(interpreter->execute(), InterpreterError);
}

BOOST_AUTO_TEST_CASE(ModuloBy0FloatFailTest) {
  std::string program = "fn main() { return 5.0%0.0; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_THROW(interpreter->execute(), InterpreterError);
}

BOOST_AUTO_TEST_CASE(DivisionByZeroFailTest) {
  std::string program =
      "fn main() { return 5/0; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_THROW(interpreter->execute(), InterpreterError);
}

BOOST_AUTO_TEST_CASE(DivisionByZeroFloatFailTest) {
  std::string program = "fn main() { return 5.0/0.0; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_THROW(interpreter->execute(), InterpreterError);
}

BOOST_AUTO_TEST_CASE(RedefinitionFailTest) {
  std::string program =
      "fn print() { return 0; } fn main() { return print(0); }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_THROW(interpreter->execute(), InterpreterError);
}

BOOST_AUTO_TEST_CASE(ReturnPrintFail) {
  std::string program =
      "fn main() { return print(0); }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_THROW(interpreter->execute(), InterpreterError);
}

BOOST_AUTO_TEST_CASE(AdditionTwoTypesFailTest) {
  std::string program = "fn main() { var a = 5.0; var b = 6; return a + b; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_THROW(interpreter->execute(), InterpreterError);
}

BOOST_AUTO_TEST_CASE(VariableScopeFailTest) {
  std::string program = "fn main() { var a = 5; mut var b; if(true) { var a = "
                        "7; b = a; } return a + b; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_THROW(interpreter->execute(), InterpreterError);
}

BOOST_AUTO_TEST_CASE(ExpressionFailTest) {
  std::string program = "fn main() { return 5.0 * 3 / 10.0; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_THROW(interpreter->execute(), InterpreterError);
}

BOOST_AUTO_TEST_CASE(ModuloFailTest) {
  std::string program = "fn main() { return 5.0 % \"test\"; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_THROW(interpreter->execute(), InterpreterError);
}

BOOST_AUTO_TEST_CASE(ReturningFunctionWithNoReturnValueFailTest) {
  std::string program =
      "fn test(var a) {} fn main() { return test(10); }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_THROW(interpreter->execute(), InterpreterError);
}

BOOST_AUTO_TEST_CASE(ReturningFunctionWithNoReturnValueFailTest2) {
  std::string program =
      "fn test(var a) { return; } fn main() { return test(10); }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_THROW(interpreter->execute(), InterpreterError);
}

BOOST_AUTO_TEST_CASE(RedefinitionInArgumentFailTest) {
  std::string program = "fn test(var a) { var a = 5; return a; } fn main() { return test(10); }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_THROW(interpreter->execute(), InterpreterError);
}

BOOST_AUTO_TEST_CASE(StringToBoolFailTest) {
  std::string program = "fn main() { return bool(\"test\"); }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_THROW(interpreter->execute(), InterpreterError);
}

BOOST_AUTO_TEST_CASE(AssignValueTwiceFailTest) {
  std::string program = "fn main() { var a = 5; a = 5.14; return a; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_THROW(interpreter->execute(), InterpreterError);
}

BOOST_AUTO_TEST_CASE(NoMainFailTest) {
  std::string program = "fn test() { var a = 5; a = 5.14; return a; }";
  auto interpreter = configureInterpreter(program);

  BOOST_CHECK_THROW(interpreter->execute(), InterpreterError);
}

BOOST_AUTO_TEST_SUITE_END()