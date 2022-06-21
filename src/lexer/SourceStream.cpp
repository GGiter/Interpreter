#include "SourceStream.h"
#include "Token.h"

SourceStream::SourceStream(const std::string_view &inStream) {
  input << inStream;
}

int SourceStream::getNextChar() {
  int nextChar = input.get();

  if (nextChar != EoLSymbol)
    currentPosition.incrementColumn();
  else
    currentPosition.incrementLine();

  return nextChar;
}

int SourceStream::peekNextChar() { return input.peek(); }

const SourcePosition &SourceStream::getCurrentPosition() const {
  return currentPosition;
}

bool SourceStream::isFile() const { return false; }
