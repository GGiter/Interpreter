#include "Function.h"
#include "Block.h"

Function::Function(const std::string &inIdentifier) : identifier(inIdentifier) {}

void Function::setIdentifer(const std::string &inIdentifier) {
  identifier = inIdentifier;
}

void Function::setBody(std::unique_ptr<Block> inBody) {
  body = std::move(inBody);
}

void Function::addArgument(
    std::unique_ptr<ParameterDefinition> inParamterDefiniton) {
  arguments.emplace_back(std::move(inParamterDefiniton));
}

const std::string &Function::getIdentifier() const { return identifier; }

Block *Function::getBlock() const { return body.get(); }

const std::vector<std::unique_ptr<ParameterDefinition>>& Function::getArguments() const {
  return arguments;
}

std::string Function::toString() const {
  std::string result = "fn " + identifier + "(";
  for (auto &argument : arguments) {
    result += argument->toString();
    if (&argument != &arguments.back())
      result += ",";
  }
  result += ")" + body->toString();
  return result;
}

std::optional<ValueType> Function::accept(VisitorInterpreter &inVisitor) const {
  return inVisitor.visit(*this);
}
