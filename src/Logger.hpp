#ifndef TOYLANG_LOGGER_HPP
#define TOYLANG_LOGGER_HPP

#include <iostream>
#include <string>

class Logger {
  public:
    static void report(int line, std::string where, std::string message);
};

#endif