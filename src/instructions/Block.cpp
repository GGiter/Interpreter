#include "Block.h"

void Block::addInstruction(std::unique_ptr<Instruction> inInstruction) {
  instructions.emplace_back(std::move(inInstruction));
}

std::string Block::toString() const {
  std::string result = "{";
  for (auto &instruction : instructions) {
    result += instruction->toString();
  }
  result += "}";
  return result;
}

const std::vector<std::unique_ptr<Instruction>> &
Block::getInstructions() const {
  return instructions;
}

std::optional<ValueType> Block::accept(VisitorInterpreter &inVisitor) const {
  return inVisitor.visit(*this);
}
