#include "Logger.hpp"

void Logger::report(int line, std::string where, std::string message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message
              << std::endl;
}