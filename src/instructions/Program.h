#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Function.h"
#include "../interpreter/VisitorInterpreter.h"

class Program {
public:
  Program() = default;
  Function *getMain() const;
  void addFunction(std::unique_ptr<Function> inFunction);
  std::string toString() const;
  const std::vector<std::unique_ptr<Function>> &getFunctions() const;
  virtual std::optional<ValueType> accept(class VisitorInterpreter &inVisitor) const;

private:
  std::vector<std::unique_ptr<Function>> functions;
};
