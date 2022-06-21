#pragma once

#include "../lexer/Token.h"
#include "Block.h"
#include "ParameterDefinition.h"
#include <memory>
#include <vector>
#include "../interpreter/VisitorInterpreter.h"

class Function {
public:
  Function() = default;
  Function(const std::string &inIdentifier);
  virtual ~Function() = default;
  void setIdentifer(const std::string &inIdentifier);
  void setBody(std::unique_ptr<Block> inBody);
  void addArgument(std::unique_ptr<ParameterDefinition> inVariable);
  const std::string &getIdentifier() const;
  Block *getBlock() const;
  const std::vector<std::unique_ptr<ParameterDefinition>>& getArguments() const;
  std::string toString() const;
  virtual std::optional<ValueType> accept(VisitorInterpreter &inVisitor) const;

protected:
  std::string identifier;

private:
  std::vector<std::unique_ptr<ParameterDefinition>> arguments;
  std::unique_ptr<Block> body;
};
