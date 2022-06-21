#include "Program.h"
#include "../interpreter/InterpreterError.h"
#include <stdexcept>

Function *Program::getMain() const {
  auto pred = [](const std::unique_ptr<Function>& function) {
    return function->getIdentifier() == "main";
  };

  auto result = std::find_if(functions.begin(), functions.end(), pred);
  if (result != functions.end())
    return (*result).get();

  throw InterpreterError("Main function does not exist!");
}

void Program::addFunction(std::unique_ptr<Function> inFunction) {
  functions.push_back(std::move(inFunction));
}

std::string Program::toString() const {
  std::string result = "";
  for (auto &function : functions) {
    result += function->toString();
  }
  return result;
}

const std::vector<std::unique_ptr<Function>> &Program::getFunctions() const {
  return functions;
}

std::optional<ValueType> Program::accept(VisitorInterpreter &inVisitor) const {
  return inVisitor.visit(*this);
}
