#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Instruction.h"
#include "../interpreter/VisitorInterpreter.h"

class Block {
public:
  Block() = default;
  void addInstruction(std::unique_ptr<Instruction> inInstruction);
  std::string toString() const;
  const std::vector<std::unique_ptr<Instruction>>& getInstructions() const;
  virtual std::optional<ValueType> accept(VisitorInterpreter &inVisitor) const;

private:
  std::vector<std::unique_ptr<Instruction>> instructions;
};
