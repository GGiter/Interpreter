#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <optional>
#include "Source.h"

class SourceFile : public Source {
public:
  explicit SourceFile(const std::string &inFileName);
  ~SourceFile();

  virtual int getNextChar() override;
  virtual int peekNextChar() override;
  virtual const SourcePosition &getCurrentPosition() const override;
  virtual bool isFile() const override;

private:
  std::string fileName;
  std::ifstream inputStream;
  SourcePosition currentPosition;
};
