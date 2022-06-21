#include "SourceFile.h"
#include "SourcePosition.h"
#include "Token.h"

SourceFile::SourceFile(const std::string &inFileName) : fileName(inFileName) {
  inputStream.open(inFileName.c_str());
  if (!inputStream) {
    throw std::runtime_error("File not found!");
  }
}

SourceFile::~SourceFile() {
  if (inputStream.is_open()) {
    inputStream.close();
  }
}

int SourceFile::getNextChar() {
  int nextChar = inputStream.get();

  if (nextChar != EoLSymbol)
    currentPosition.incrementColumn();
  else
    currentPosition.incrementLine();

  return nextChar;
}

int SourceFile::peekNextChar() { return inputStream.peek(); }

const SourcePosition &SourceFile::getCurrentPosition() const {
  return currentPosition;
}

bool SourceFile::isFile() const { return true; }
