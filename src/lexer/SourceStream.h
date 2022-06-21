#pragma once
#include "Source.h"
#include <sstream>
#include <string>
#include <string_view>

class SourceStream : public Source {
public:
  explicit SourceStream(const std::string_view &inStream);

  virtual int getNextChar() override;
  virtual int peekNextChar() override;
  virtual const SourcePosition &getCurrentPosition() const override;
  virtual bool isFile() const override;

private:
  std::stringstream input;
  SourcePosition currentPosition;
};
