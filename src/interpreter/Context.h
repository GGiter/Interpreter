#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <optional>
#include "InterpreterValue.h"

typedef std::pair<std::variant<std::string, float, int, bool>, Token::Type> ValueType;
typedef std::unordered_map<std::string, InterpreterValue>
    VariablesMap;

template <class... Ts> struct overload : Ts... { using Ts::operator()...; };
template <class... Ts>
overload(Ts...) -> overload<Ts...>;


class Context {
public:
  friend class VisitorInterpreterImpl;
  Context();
  bool containsVariableWithName(const std::string &inName) const;
  std::optional<InterpreterValue> findVariableWithName(const std::string &inName) const;
  void insertLocalVariable(const std::string &inName,
                           InterpreterValue& inValue);
  void replaceLocalVariable(const std::string &inName,
                           InterpreterValue& inValue);
  const class Function *findFunction(const std::string &inName) const;
  void insertFunction(const class Function *inFunction);
  VariablesMap copyCurrentVariablesMap();
  void reset();

private:
  void createPrintFunction();
  void createIntFunction();
  void createFloatFunction();
  void createStringFunction();
  void createBoolFunction();

  std::vector<class Expression *> argList;
  std::optional<std::string> matchVariableName;
  const class Function *currentFunction;
  const class Function *previousFunction;
  std::unordered_map<const class Function *, VariablesMap> localVariables;
  std::vector<const class Function *> functionList;

  std::vector<std::unique_ptr<class Function>> internalFunctionList;
};
