#pragma once
#include <string>
#include <memory>

class ParameterDefinition {
public:
  explicit ParameterDefinition(const std::string &inName,
                               bool bInIsMutable = false);
  std::string toString() const;
  const std::string &getName() const;
  bool isMutable() const;

private:
  std::string name;
  bool bIsMutable;
};
