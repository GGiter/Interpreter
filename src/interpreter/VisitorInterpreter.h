#pragma once
#include "Context.h"

class VisitorInterpreter {
public:
  VisitorInterpreter() = default;
  virtual ~VisitorInterpreter() = default;
  virtual std::optional<ValueType> execute() = 0;
  virtual std::optional<ValueType>
  visit(const class Program &inProgram) = 0;
  virtual std::optional<ValueType>
  visit(const class BinaryExpression &inBinaryExpression) = 0;
  virtual std::optional<ValueType> visit(const class Block &inBlock) = 0;
  virtual std::optional<ValueType> visit(const class Case &inCase) = 0;
  virtual std::optional<ValueType>
  visit(const class Function &inFunction) = 0;
  virtual std::optional<ValueType> visit(
      const class FunctionCallExpression &inFunctionCallExpression) = 0;
  virtual std::optional<ValueType>
  visit(const class IfElse &inIfElse) = 0;
  virtual std::optional<ValueType>
  visit(const class InstructionAssigment &inAssigment) = 0;
  virtual std::optional<ValueType> visit(
      const class InstructionDeclarationVariable &inDeclarationVariable) = 0;
  virtual std::optional<ValueType>
  visit(const class InstructionFunctionCall &inFunctionCall) = 0;
  virtual std::optional<ValueType>
  visit(const class PrintFunction &inPrintFunction) = 0;
  virtual std::optional<ValueType>
  visit(const class IntFunction &inIntFunction) = 0;
  virtual std::optional<ValueType>
  visit(const class StringFunction &inStringFunction) = 0;
  virtual std::optional<ValueType>
  visit(const class FloatFunction &inFloatFunction) = 0;
  virtual std::optional<ValueType>
  visit(const class BoolFunction &inBoolFunction) = 0;
  virtual std::optional<ValueType>
  visit(const class InstructionReturn &inReturn) = 0;
  virtual std::optional<ValueType> visit(const class Match &inMatch) = 0;
  virtual std::optional<ValueType>
  visit(const class UnaryExpression &inUnaryExpression) = 0;
  virtual std::optional<ValueType> visit(
      const class VariableExpression &inVariableExpression) = 0;
  virtual std::optional<ValueType> visit(const class While &inWhile) = 0;
};
