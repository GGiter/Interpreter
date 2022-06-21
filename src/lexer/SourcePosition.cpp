#include "SourcePosition.h"

SourcePosition::SourcePosition(unsigned int inLine, unsigned int inColumn)
    : line(inLine), column(inColumn) {}

void SourcePosition::incrementColumn() { column++; }

void SourcePosition::incrementLine() {
  line++;
  column = 0;
}

int SourcePosition::getLine() const { return line; }

int SourcePosition::getColumn() const { return column; }

std::string SourcePosition::toString() const {
  return "line: " + std::to_string(line) + " column: " + std::to_string(column);
}
