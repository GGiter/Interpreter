#include "Context.h"
#include "../instructions/Expression.h"
#include "../instructions/Function.h"
#include "../instructions/Variable.h"
#include "../instructions/PrintFunction.h"
#include "../instructions/IntFunction.h"
#include "../instructions/FloatFunction.h"
#include "../instructions/StringFunction.h"
#include "../instructions/BoolFunction.h"
#include "../instructions/ParameterDefinition.h"

Context::Context() : currentFunction(nullptr), previousFunction(nullptr) {
  createPrintFunction();
  createIntFunction();
  createFloatFunction();
  createBoolFunction();
  createStringFunction();
}

bool Context::containsVariableWithName(const std::string &inName) const {
  const auto &it = localVariables.find(currentFunction)->second;
  return it.contains(inName);
}

std::optional<InterpreterValue>
Context::findVariableWithName(const std::string &inName) const {
  const auto &functionIt = localVariables.find(currentFunction)->second;
  auto it = functionIt.find(inName);
  if (it != functionIt.end())
    return it->second;

  return std::nullopt;
}

void Context::insertLocalVariable(const std::string &inName,
                                  InterpreterValue& inValue) {
  localVariables[currentFunction].insert(
      std::make_pair(inName, inValue));
}

void Context::replaceLocalVariable(const std::string &inName,
                                   InterpreterValue& inValue) {
  auto it = localVariables[currentFunction].find(inName);
  if (it != localVariables[currentFunction].end()) {
    it->second = inValue;
  }
}

const Function *Context::findFunction(const std::string &inName) const {
  auto pred = [&inName](const Function *function) {
    return function->getIdentifier() == inName;
  };

  auto result = std::find_if(functionList.begin(), functionList.end(), pred);
  if (result != functionList.end())
    return *result;

  return nullptr;
}

void Context::insertFunction(const Function *inFunction) {
  functionList.push_back(inFunction);
  localVariables.insert(std::make_pair(inFunction, VariablesMap()));
}

VariablesMap Context::copyCurrentVariablesMap() {
  auto copyOfLocalVariables = VariablesMap();
  for (const auto &pair : localVariables[currentFunction]) {
    copyOfLocalVariables.insert(std::make_pair(
        pair.first, InterpreterValue(
                        pair.second.getType(), pair.second.getValue(),
                        pair.second.isMutable())));
  }
  return copyOfLocalVariables;
}

void Context::reset() {
  previousFunction = nullptr;
  currentFunction = nullptr;
  matchVariableName = std::nullopt;
  localVariables.clear();
  functionList.clear();
  argList.clear();

  for (const auto& internalFunction : internalFunctionList) {
    functionList.emplace_back(internalFunction.get());
  }
}

void Context::createPrintFunction() {
  auto function = std::make_unique<PrintFunction>();
  function->addArgument(std::make_unique<ParameterDefinition>("input"));
  functionList.emplace_back(function.get());
  internalFunctionList.emplace_back(std::move(function));
}

void Context::createIntFunction() {
  auto function = std::make_unique<IntFunction>();
  function->addArgument(std::make_unique<ParameterDefinition>("input"));
  functionList.emplace_back(function.get());
  internalFunctionList.emplace_back(std::move(function));
}

void Context::createFloatFunction() {
  auto function = std::make_unique<FloatFunction>();
  function->addArgument(std::make_unique<ParameterDefinition>("input"));
  functionList.emplace_back(function.get());
  internalFunctionList.emplace_back(std::move(function));
}

void Context::createStringFunction() {
  auto function = std::make_unique<StringFunction>();
  function->addArgument(std::make_unique<ParameterDefinition>("input"));
  functionList.emplace_back(function.get());
  internalFunctionList.emplace_back(std::move(function));
}

void Context::createBoolFunction() {
  auto function = std::make_unique<BoolFunction>();
  function->addArgument(std::make_unique<ParameterDefinition>("input"));
  functionList.emplace_back(function.get());
  internalFunctionList.emplace_back(std::move(function));
}
