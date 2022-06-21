#include "lexer/Lexer.h"
#include "lexer/SourceFile.h"
#include "parser/Parser.h"
#include "interpreter/VisitorInterpreter.h"
#include "interpreter/VisitorInterpreterImpl.h"
#include <iostream>

int main(int argc, char **argv) {
  std::unique_ptr<Source> source;
  std::unique_ptr<Lexer> lexer;
  std::unique_ptr<Parser> parser;
  std::unique_ptr<VisitorInterpreter> interpreter;

  if (argc > 1)
    try {
      source = std::make_unique<SourceFile>(argv[1]);
    } catch (const std::runtime_error &error) {
      std::cout << "Source error: " << error.what() << std::endl;
      return -1;
    }
  else {
      std::cout << "Program requires path to file as an argument!" << std::endl;
      return -1;
  }

  try {
    lexer = std::make_unique<Lexer>(std::move(source));
  } catch (const std::runtime_error &error) {
    std::cout << "Lexer error: " << error.what() << std::endl;
    return -1;
  }

  try {
    parser = std::make_unique<Parser>(std::move(lexer));
    parser->parseProgram();
  } catch (const std::runtime_error &error) {
    std::cout << "Parser error: " << error.what() << std::endl;
    return -1;
  }

   try {
    interpreter = std::make_unique<VisitorInterpreterImpl>(std::move(parser));
    auto returnValue = interpreter->execute();
    if (returnValue.has_value()) {
      std::visit(overload{
                     [](const auto &in) { std::cout << in; },
                     [](bool in) { std::cout << std::to_string(in); },
                 },
                 returnValue->first);
    }
  } catch (const std::runtime_error &error) {
    std::cout << "Interpreter error: " << error.what() << std::endl;
    return -1;
  }


  return 0;
}