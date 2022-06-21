#include "VisitorInterpreterImpl.h"
#include "../instructions/BinaryExpression.h"
#include "../instructions/Block.h"
#include "../instructions/Case.h"
#include "../instructions/Function.h"
#include "../instructions/FunctionCallExpression.h"
#include "../instructions/IfElse.h"
#include "../instructions/InstructionAssigment.h"
#include "../instructions/InstructionDeclarationVariable.h"
#include "../instructions/InstructionFunctionCall.h"
#include "../instructions/PrintFunction.h"
#include "../instructions/IntFunction.h"
#include "../instructions/StringFunction.h"
#include "../instructions/FloatFunction.h"
#include "../instructions/BoolFunction.h"
#include "../instructions/InstructionReturn.h"
#include "../instructions/Match.h"
#include "../instructions/ParameterDefinition.h"
#include "../instructions/UnaryExpression.h"
#include "../instructions/Value.h"
#include "../instructions/Variable.h"
#include "../instructions/VariableExpression.h"
#include "../instructions/While.h"
#include "../lexer/Lexer.h"
#include "../parser/Parser.h"
#include "InterpreterError.h"
#include <cmath>
#include <iostream>

VisitorInterpreterImpl::VisitorInterpreterImpl(std::unique_ptr<Parser> inParser)
    : parser(std::move(inParser)) {}

std::optional<ValueType> VisitorInterpreterImpl::execute() {
  context.reset();
  Program *program = parser->parseProgram();
  return program->accept(*this);
}

std::optional<ValueType>
VisitorInterpreterImpl::visit(const Program &inProgram) {
  for (const auto &function : inProgram.getFunctions()) {
    const std::string &functionName = function->getIdentifier();
    if (context.findFunction(functionName) != nullptr)
      throw InterpreterError("Redefinition of function with name " +
                             functionName + "!");
    context.insertFunction(function.get());
  }

  if (auto *main = inProgram.getMain()) {
    return main->accept(*this);
  } else {
    throw InterpreterError("No function with name main found!");
  }
  return std::nullopt;
}

std::optional<ValueType>
VisitorInterpreterImpl::visit(const BinaryExpression &inBinaryExpression) {
  auto lhsValue = inBinaryExpression.getLhs()->accept(*this);
  auto rhsValue = inBinaryExpression.getRhs()->accept(*this);
  switch (inBinaryExpression.getOperator()) {
  case Expression::Operator::Sum: {
      return std::visit(
          overload{
              [](int lhs, int rhs) {
                return (ValueType)std::make_pair(lhs + rhs, Token::Type::IntLiteral);
              },
              [](float lhs, float rhs) {
                return (ValueType)std::make_pair(lhs + rhs,
                                                 Token::Type::FloatLiteral);
              },
              [](std::string &lhs, std::string &rhs) {
                return (ValueType)std::make_pair(lhs + rhs,
                                                 Token::Type::StringLiteral);
              },
              [](const auto &lhs, const auto &rhs) {
                throw InterpreterError("Cannot calculate (+)!");
                return (ValueType)std::make_pair(0, Token::Type::BadType);
              },
          },
          lhsValue->first, rhsValue->first);
  }
  case Expression::Operator::Substraction: {
    return std::visit(
        overload{
            [](int lhs, int rhs) {
              return (ValueType)std::make_pair(lhs - rhs,
                                               Token::Type::IntLiteral);
            },
            [](float lhs, float rhs) {
              return (ValueType)std::make_pair(lhs - rhs,
                                               Token::Type::FloatLiteral);
            },
            [](const auto &lhs, const auto &rhs) {
              throw InterpreterError("Cannot calculate (-)!");
              return (ValueType)std::make_pair(0, Token::Type::BadType);
            },
        },
        lhsValue->first, rhsValue->first);
  }
  case Expression::Operator::Multiplication: {
    return std::visit(
        overload{
            [](int lhs, int rhs) {
              return (ValueType)std::make_pair(lhs * rhs,
                                               Token::Type::IntLiteral);
            },
            [](float lhs, float rhs) {
              return (ValueType)std::make_pair(lhs * rhs,
                                               Token::Type::FloatLiteral);
            },
            [](const auto &lhs, const auto &rhs) {
              throw InterpreterError("Cannot calculate (*)!");
              return (ValueType)std::make_pair(0, Token::Type::BadType);
            },
        },
        lhsValue->first, rhsValue->first);
  }
  case Expression::Operator::Division: {
    return std::visit(
        overload{
            [](int lhs, int rhs) {
              if (rhs == 0)
                throw InterpreterError("Cannot divide by 0!");
              return (ValueType)std::make_pair(lhs / rhs,
                                               Token::Type::IntLiteral);
            },
            [](float lhs, float rhs) {
              if (rhs == 0.0)
                throw InterpreterError("Cannot divide by 0.0!");
              return (ValueType)std::make_pair(lhs / rhs,
                                               Token::Type::FloatLiteral);
            },
            [](const auto &lhs, const auto &rhs) {
              throw InterpreterError("Cannot calculate (/)!");
              return (ValueType)std::make_pair(0, Token::Type::BadType);
            },
        },
        lhsValue->first, rhsValue->first);
  }
  case Expression::Operator::Modulo: {
    return std::visit(
        overload{
            [](int lhs, int rhs) {
              if (rhs == 0)
                throw InterpreterError("Cannot modulo by 0!");
              return (ValueType)std::make_pair(lhs % rhs,
                                               Token::Type::IntLiteral);
            },
            [](float lhs, float rhs) {
              if (rhs == 0.0)
                throw InterpreterError("Cannot modulo by 0.0!");
              return (ValueType)std::make_pair(fmod(lhs,rhs),
                                               Token::Type::FloatLiteral);
            },
            [](const auto &lhs, const auto &rhs) {
              throw InterpreterError("Cannot calculate (%)!");
              return (ValueType)std::make_pair(0, Token::Type::BadType);
            },
        },
        lhsValue->first, rhsValue->first);
  }
  case Expression::Operator::LogicalOr: {
    return std::visit(
        overload{
            [](int lhs, int rhs) {
              return (ValueType)std::make_pair(lhs || rhs,
                                               Token::Type::BooleanLiteral);
            },
            [](float lhs, float rhs) {
              return (ValueType)std::make_pair(lhs || rhs,
                                               Token::Type::BooleanLiteral);
            },
            [](bool lhs, bool rhs) {
              return (ValueType)std::make_pair(lhs || rhs,
                                               Token::Type::BooleanLiteral);
            },
            [](const auto &lhs, const auto &rhs) {
              throw InterpreterError("Cannot calculate (||)!");
              return (ValueType)std::make_pair(0, Token::Type::BadType);
            },
        },
        lhsValue->first, rhsValue->first);
  }
  case Expression::Operator::LogicalAnd: {
    return std::visit(
        overload{
            [](int lhs, int rhs) {
              return (ValueType)std::make_pair(lhs && rhs,
                                               Token::Type::BooleanLiteral);
            },
            [](float lhs, float rhs) {
              return (ValueType)std::make_pair(lhs && rhs,
                                               Token::Type::BooleanLiteral);
            },
            [](bool lhs, bool rhs) {
              return (ValueType)std::make_pair(lhs && rhs,
                                               Token::Type::BooleanLiteral);
            },
            [](const auto &lhs, const auto &rhs) {
              throw InterpreterError("Cannot calculate (&&)!");
              return (ValueType)std::make_pair(0, Token::Type::BadType);
            },
        },
        lhsValue->first, rhsValue->first);
  }
  case Expression::Operator::Less: {
    return std::visit(
        overload{
            [](int lhs, int rhs) {
              return (ValueType)std::make_pair(lhs < rhs,
                                               Token::Type::BooleanLiteral);
            },
            [](float lhs, float rhs) {
              return (ValueType)std::make_pair(lhs < rhs,
                                               Token::Type::BooleanLiteral);
            },
            [](const auto &lhs, const auto &rhs) {
              throw InterpreterError("Cannot calculate (<)!");
              return (ValueType)std::make_pair(0, Token::Type::BadType);
            },
        },
        lhsValue->first, rhsValue->first);
  }
  case Expression::Operator::LessEqual: {
    return std::visit(
        overload{
            [](int lhs, int rhs) {
              return (ValueType)std::make_pair(lhs <= rhs,
                                               Token::Type::BooleanLiteral);
            },
            [](float lhs, float rhs) {
              return (ValueType)std::make_pair(lhs <= rhs,
                                               Token::Type::BooleanLiteral);
            },
            [](const auto &lhs, const auto &rhs) {
              throw InterpreterError("Cannot calculate (<=)!");
              return (ValueType)std::make_pair(0, Token::Type::BadType);
            },
        },
        lhsValue->first, rhsValue->first);
  }
  case Expression::Operator::More: {
    return std::visit(
        overload{
            [](int lhs, int rhs) {
              return (ValueType)std::make_pair(lhs > rhs,
                                               Token::Type::BooleanLiteral);
            },
            [](float lhs, float rhs) {
              return (ValueType)std::make_pair(lhs > rhs,
                                               Token::Type::BooleanLiteral);
            },
            [](const auto &lhs, const auto &rhs) {
              throw InterpreterError("Cannot calculate (>)!");
              return (ValueType)std::make_pair(0, Token::Type::BadType);
            },
        },
        lhsValue->first, rhsValue->first);
  }
  case Expression::Operator::MoreEqual: {
    return std::visit(
        overload{
            [](int lhs, int rhs) {
              return (ValueType)std::make_pair(lhs >= rhs,
                                               Token::Type::BooleanLiteral);
            },
            [](float lhs, float rhs) {
              return (ValueType)std::make_pair(lhs >= rhs,
                                               Token::Type::BooleanLiteral);
            },
            [](const auto &lhs, const auto &rhs) {
              throw InterpreterError("Cannot calculate (>=)!");
              return (ValueType)std::make_pair(0, Token::Type::BadType);
            },
        },
        lhsValue->first, rhsValue->first);
  }
  case Expression::Operator::Equal: {
    return std::visit(
        overload{
            [](int lhs, int rhs) {
              return (ValueType)std::make_pair(lhs == rhs,
                                               Token::Type::BooleanLiteral);
            },
            [](float lhs, float rhs) {
              return (ValueType)std::make_pair(lhs == rhs,
                                               Token::Type::BooleanLiteral);
            },
            [](bool lhs, bool rhs) {
              return (ValueType)std::make_pair(lhs == rhs,
                                               Token::Type::BooleanLiteral);
            },
            [](std::string& lhs, std::string& rhs) {
              return (ValueType)std::make_pair(lhs == rhs,
                                               Token::Type::BooleanLiteral);
            },
            [](const auto &lhs, const auto &rhs) {
              throw InterpreterError("Cannot calculate (==)!");
              return (ValueType)std::make_pair(0, Token::Type::BadType);
            },
        },
        lhsValue->first, rhsValue->first);
  }
  case Expression::Operator::NotEqual: {
    return std::visit(
        overload{
            [](int lhs, int rhs) {
              return (ValueType)std::make_pair(lhs != rhs,
                                               Token::Type::BooleanLiteral);
            },
            [](float lhs, float rhs) {
              return (ValueType)std::make_pair(lhs != rhs,
                                               Token::Type::BooleanLiteral);
            },
            [](bool lhs, bool rhs) {
              return (ValueType)std::make_pair(lhs != rhs,
                                               Token::Type::BooleanLiteral);
            },
            [](std::string &lhs, std::string &rhs) {
              return (ValueType)std::make_pair(lhs != rhs,
                                               Token::Type::BooleanLiteral);
            },
            [](const auto &lhs, const auto &rhs) {
              throw InterpreterError("Cannot calculate (!=)!");
              return (ValueType)std::make_pair(0, Token::Type::BadType);
            },
        },
        lhsValue->first, rhsValue->first);
  }
  }
  return std::nullopt;
}

std::optional<ValueType> VisitorInterpreterImpl::visit(const Block &inBlock) {
  for (auto &instruction : inBlock.getInstructions()) {
    auto returnValue = instruction->accept(*this);
    if (returnValue.has_value())
      return returnValue;
  }
  return std::nullopt;
}

std::optional<ValueType> VisitorInterpreterImpl::visit(const Case &inCase) {
  return inCase.getBlock()->accept(*this);
}

std::optional<ValueType>
VisitorInterpreterImpl::visit(const Function &inFunction) {
  std::vector<InterpreterValue> values;
  for (size_t i = 0; i < context.argList.size(); ++i) {
    auto result = context.argList[i]->accept(*this);
    InterpreterValue value(result->second, result->first,
                           inFunction.getArguments()[i]->isMutable());
    values.emplace_back(value);
  }

  /* Default values */
  for (size_t i = context.argList.size(); i < inFunction.getArguments().size();
       ++i) {
    InterpreterValue value(Token::Type::IntLiteral, 0,
                           inFunction.getArguments()[i]->isMutable());
    values.emplace_back(value);
  }

  /* Store and clear previous variable scope */
  context.previousFunction = context.currentFunction;
  context.currentFunction = &inFunction;
  auto copyOfLocalVariables = std::move(context.copyCurrentVariablesMap());
  context.localVariables[&inFunction].clear();
  for (size_t i = 0; i < inFunction.getArguments().size(); ++i) {
    context.insertLocalVariable(inFunction.getArguments()[i]->getName(),
                                values[i]);
  }

  /* Return previous variable scope */
  auto returnValue = inFunction.getBlock()->accept(*this);
  context.localVariables[&inFunction] = std::move(copyOfLocalVariables);
  context.currentFunction = context.previousFunction;
  return returnValue;
}

std::optional<ValueType> VisitorInterpreterImpl::visit(
    const FunctionCallExpression &inFunctionCallExpression) {
  auto returnValue = static_cast<const InstructionFunctionCall *>(
                         inFunctionCallExpression.getFunctionCall())
                         ->accept(*this);
  if (!returnValue.has_value())
    throw InterpreterError("FunctionCallExpression has to return value");

  return returnValue;
}

std::optional<ValueType> VisitorInterpreterImpl::visit(const IfElse &inIfElse) {
  auto result = inIfElse.getExpression()->accept(*this);
  if (result.has_value() && result->second == Token::Type::BooleanLiteral &&
      std::get<bool>(result->first)) {
    return inIfElse.getBlockIf()->accept(*this);
  } else {
    return inIfElse.getBlockElse()->accept(*this);
  }
}

std::optional<ValueType>
VisitorInterpreterImpl::visit(const InstructionAssigment &inAssigment) {
  const std::string name = inAssigment.getVariable()->toString();
  if (!context.containsVariableWithName(name))
    throw InterpreterError("Variable " + name + " is not declared!");

  const auto &variable = context.findVariableWithName(name);
  if (!variable->isMutable())
    throw InterpreterError("Not mutable variable " + name +
                           " cannot be modified!");

  auto result = inAssigment.getExpression()->accept(*this);
  InterpreterValue value(result->second, result->first, variable->isMutable());
  context.replaceLocalVariable(name, value);
  return std::nullopt;
}

std::optional<ValueType> VisitorInterpreterImpl::visit(
    const InstructionDeclarationVariable &inDeclarationVariable) {
  std::string name = inDeclarationVariable.getIdentifier();
  if (context.containsVariableWithName(name))
    throw InterpreterError("New declaration of local variable named " + name +
                           " found!");

  if (inDeclarationVariable.getExpression()) {
    auto result = inDeclarationVariable.getExpression()->accept(*this);
    InterpreterValue value(result->second, result->first,
                           inDeclarationVariable.isMutable());
    context.insertLocalVariable(name, value);
  } else {
    InterpreterValue value(Token::Type::IntLiteral, 0,
                           inDeclarationVariable.isMutable());
    context.insertLocalVariable(name, value);
  }

  return std::nullopt;
}

std::optional<ValueType>
VisitorInterpreterImpl::visit(const InstructionFunctionCall &inFunctionCall) {
  std::string name = inFunctionCall.getFunctionName();
  auto function = context.findFunction(name);
  if (function == nullptr)
    throw InterpreterError("No function found with such name: " + name + "!");
  if (function->getArguments().size() != inFunctionCall.getExpressions().size())
    throw InterpreterError("Invalid number of arguments for function " + name +
                           "!");

  context.argList.clear();
  for (const auto &argument : inFunctionCall.getExpressions()) {
    context.argList.push_back(argument.get());
  }
  return function->accept(*this);

  return std::nullopt;
}

std::optional<ValueType>
VisitorInterpreterImpl::visit(const IntFunction &inIntFunction) {
  auto expressionValue = context.argList[0]->accept(*this);
  return std::visit(
      overload{
          [](std::string &in) {
            return ValueType(stoi(in), Token::Type::IntLiteral);
          },
          [](float in) { return ValueType((int)in, Token::Type::IntLiteral); },
          [expressionValue](int in) { return *expressionValue; },
          [](bool in) { return ValueType((int)in, Token::Type::IntLiteral); },
      },
      expressionValue->first);
}

std::optional<ValueType>
VisitorInterpreterImpl::visit(const StringFunction &inStringFunction) {
  auto expressionValue = context.argList[0]->accept(*this);
  return std::visit(
      overload{
          [expressionValue](std::string &in) { return *expressionValue; },
          [](float in) {
            return ValueType(std::to_string(in), Token::Type::StringLiteral);
          },
          [](int in) {
            return ValueType(std::to_string(in), Token::Type::StringLiteral);
          },
          [](bool in) {
            return ValueType(in == true ? "true" : "false",
                             Token::Type::StringLiteral);
          },
      },
      expressionValue->first);
}

std::optional<ValueType>
VisitorInterpreterImpl::visit(const FloatFunction &inFloatFunction) {
  auto expressionValue = context.argList[0]->accept(*this);
  return std::visit(
      overload{
          [](std::string &in) {
            return ValueType(std::stof(in), Token::Type::FloatLiteral);
          },
          [expressionValue](float in) { return *expressionValue; },
          [](int in) {
            return ValueType((float)in, Token::Type::FloatLiteral);
          },
          [](bool in) {
            return ValueType((float)in, Token::Type::FloatLiteral);
          },
      },
      expressionValue->first);
}

std::optional<ValueType>
VisitorInterpreterImpl::visit(const BoolFunction &inBoolFunction) {
  auto expressionValue = context.argList[0]->accept(*this);
  return std::visit(
      overload{
          [](std::string &in) {
            if (in == "true")
              return ValueType(true, Token::Type::BooleanLiteral);
            if (in == "false")
              return ValueType(false, Token::Type::BooleanLiteral);
            throw InterpreterError("Cannot convert string to bool!");
          },
          [](float in) {
            return ValueType(in > 0.0 ? true : false,
                             Token::Type::BooleanLiteral);
          },
          [](int in) {
            return ValueType(in > 0 ? true : false,
                             Token::Type::BooleanLiteral);
          },
          [expressionValue](bool in) { return *expressionValue; },
      },
      expressionValue->first);
}

std::optional<ValueType>
VisitorInterpreterImpl::visit(const PrintFunction &inPrintFunction) {
  auto expressionValue = context.argList[0]->accept(*this);
  std::visit(overload{
                 [](std::string &in) { std::cout << in; },
                 [](float in) { std::cout << in; },
                 [](int in) { std::cout << in; },
                 [](bool in) { std::cout << std::to_string(in); },
             },
             expressionValue->first);
  return std::nullopt;
}

std::optional<ValueType>
VisitorInterpreterImpl::visit(const InstructionReturn &inReturn) {
  if (inReturn.getExpression())
    return inReturn.getExpression()->accept(*this);
  return std::nullopt;
}

std::optional<ValueType> VisitorInterpreterImpl::visit(const Match &inMatch) {
  auto result = inMatch.getExpression()->accept(*this);
  auto returnValue = std::optional<ValueType>(std::nullopt);
  context.matchVariableName = inMatch.getExpression()->toString();

  const auto &localVariable =
      context.findVariableWithName(*context.matchVariableName);

  /* Input variable to the context if it doesn't exists */
  if (!localVariable) {
    InterpreterValue value(result->second, result->first, false);
    context.insertLocalVariable(*context.matchVariableName, value);
  }

  for (const auto &caseInstruction : inMatch.getCases()) {
    auto caseExpressionResult = caseInstruction->getExpression()->accept(*this);
    if ((caseExpressionResult->second == Token::Type::BooleanLiteral &&
         std::get<bool>(caseExpressionResult->first)) ||
        (*result == *caseExpressionResult)) {
      auto caseReturn = caseInstruction->accept(*this);
      if (caseReturn.has_value()) {
        returnValue = caseReturn;
      }
      break;
    }
  }
  context.matchVariableName = std::nullopt;
  return returnValue;
}

std::optional<ValueType>
VisitorInterpreterImpl::visit(const UnaryExpression &inUnaryExpression) {
  auto result = inUnaryExpression.getExpression()->accept(*this);
  if (result->second == Token::Type::FloatLiteral) {
    return std::make_pair(-std::get<float>(result->first), result->second);
  }
  if (result->second == Token::Type::IntLiteral) {
    return std::make_pair(-std::get<int>(result->first), result->second);
  }
  if (result->second == Token::Type::BooleanLiteral) {
    if (std::get<bool>(result->first))
      return std::make_pair(false, result->second);
    return std::make_pair(true, result->second);
  }
  throw InterpreterError("Invalid expression type!");
}

std::optional<ValueType>
VisitorInterpreterImpl::visit(const VariableExpression &inVariableExpression) {
  const auto &variable = inVariableExpression.getVariable();
  if (const auto &value = variable->getValue()) {
    return std::make_pair(value->getValue(), value->getType());
  }

  auto name = *inVariableExpression.getVariable()->getName();
  if (name == "_" && context.matchVariableName.has_value()) {
    name = *context.matchVariableName;
  }

  const auto &localVariable = context.findVariableWithName(name);

  if (!localVariable)
    throw InterpreterError("No variable with such name " +
                           *inVariableExpression.getVariable()->getName() +
                           "!");

  return std::make_pair(localVariable->getValue(), localVariable->getType());
}

std::optional<ValueType> VisitorInterpreterImpl::visit(const While &inWhile) {
  auto result = inWhile.getExpression()->accept(*this);
  if (result->second == Token::Type::BooleanLiteral) {
    while (isWhileExpressionTrue(*result)) {
      auto returnValue = inWhile.getBody()->accept(*this);
      result = inWhile.getExpression()->accept(*this);
      if (returnValue.has_value())
        return returnValue;
    }
  } else {
    throw InterpreterError("Invalid expression type in while!");
  }
  return std::nullopt;
}

bool VisitorInterpreterImpl::isWhileExpressionTrue(
    const ValueType &inValueType) const {
  if (inValueType.second == Token::Type::StringLiteral) {
    auto stringValue = std::get<std::string>(inValueType.first);
    if (!context.containsVariableWithName(stringValue))
      throw InterpreterError(
          "No variable with such name as in while expression!");

    const auto &variable = context.findVariableWithName(stringValue);
    return isValueTypeTrue(
        std::make_pair(variable->getValue(), variable->getType()));
  }

  return isValueTypeTrue(inValueType);
}

bool VisitorInterpreterImpl::isValueTypeTrue(
    const ValueType &inValueType) const {
  return std::visit(overload{
                        [](std::string in) { return false; },
                        [](float in) { return (bool)in; },
                        [](int in) { return (bool)in; },
                        [](bool in) { return in; },
                    },
                    inValueType.first);
}
