#pragma once
#include "SourcePosition.h"
#include <memory>

class Source {
public:
  virtual ~Source() = default;
  virtual int getNextChar() = 0;
  virtual int peekNextChar() = 0;
  virtual const SourcePosition &getCurrentPosition() const = 0;
  virtual bool isFile() const = 0;
};
