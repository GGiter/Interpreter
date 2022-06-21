#include "ParameterDefinition.h"

ParameterDefinition::ParameterDefinition(const std::string &inName,
                                         bool bInIsMutable)
    : name(inName), bIsMutable(bInIsMutable) {}

std::string ParameterDefinition::toString() const {
  std::string result = "";
  if (bIsMutable) {
    result += "mut ";
  }
  result += "var " + name;
  return result;
}

const std::string &ParameterDefinition::getName() const { return name; }

bool ParameterDefinition::isMutable() const { return bIsMutable; }