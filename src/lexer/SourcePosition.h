#pragma once
#include <string>

class SourcePosition {
public:
  SourcePosition(unsigned int inLine = 0, unsigned int inColumn = 0);

  void incrementColumn();
  void incrementLine();
  int getLine() const;
  int getColumn() const;
  std::string toString() const;

private:
  unsigned int line;
  unsigned int column;
};
