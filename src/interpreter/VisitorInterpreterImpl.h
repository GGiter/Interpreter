#pragma once

#include "VisitorInterpreter.h"
#include <optional>
#include <memory>

class VisitorInterpreterImpl : public VisitorInterpreter {
public:
  explicit VisitorInterpreterImpl(std::unique_ptr<class Parser> inParser);
  virtual std::optional<ValueType> execute() override;
  virtual std::optional<ValueType>
  visit(const class Program &inProgram) override;
  virtual std::optional<ValueType>
  visit(const class BinaryExpression &inBinaryExpression) override;
  virtual std::optional<ValueType> visit(const class Block &inBlock) override;
  virtual std::optional<ValueType> visit(const class Case &inCase) override;
  virtual std::optional<ValueType>
  visit(const class Function &inFunction) override;
  virtual std::optional<ValueType> visit(
      const class FunctionCallExpression &inFunctionCallExpression) override;
  virtual std::optional<ValueType> visit(const class IfElse &inIfElse) override;
  virtual std::optional<ValueType>
  visit(const class InstructionAssigment &inAssigment) override;
  virtual std::optional<ValueType> visit(const class InstructionDeclarationVariable &inDeclarationVariable)
      override;
  virtual std::optional<ValueType>
  visit(const class InstructionFunctionCall &inFunctionCall) override;
  virtual std::optional<ValueType>
  visit(const class IntFunction &inIntFunction) override;
  virtual std::optional<ValueType>
  visit(const class StringFunction &inStringFunction) override;
  virtual std::optional<ValueType>
  visit(const class FloatFunction &inFloatFunction) override;
  virtual std::optional<ValueType>
  visit(const class BoolFunction &inBoolFunction) override;
  virtual std::optional<ValueType>
  visit(const class PrintFunction &inPrintFunction) override;
  virtual std::optional<ValueType>
  visit(const class InstructionReturn &inReturn) override;
  virtual std::optional<ValueType> visit(const class Match &inMatch) override;
  virtual std::optional<ValueType>
  visit(const class UnaryExpression &inUnaryExpression) override;
  virtual std::optional<ValueType>
  visit(const class VariableExpression &inVariableExpression) override;
  virtual std::optional<ValueType> visit(const class While &inWhile) override;

private:
  bool isWhileExpressionTrue(const ValueType &inValueType) const;
  bool isValueTypeTrue(const ValueType &inValueType) const;
  std::unique_ptr<Parser> parser;
  Context context;
};
