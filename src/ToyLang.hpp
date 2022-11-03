#ifndef TOYLANG_HPP
#define TOYLANG_HPP

#include "Compiler.hpp"
#include "Lexer.hpp"
#include "Logger.hpp"
#include "Parser.hpp"
#include "StringifyAST.hpp"
#include "Token.hpp"

#include "llvm/IR/Value.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class ToyLang {
  public:
    static void runFile(const char *path);
    static void runPrompt();

    static void error(int line, std::string message);
    static void error(Token token, std::string message);

  private:
    static bool hadError;

    static void run(const char *source);
};

#endif