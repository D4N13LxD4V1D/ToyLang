#ifndef TOYLANG_LEXER_HPP
#define TOYLANG_LEXER_HPP

#include <map>
#include <string>
#include <vector>

#include "Token.hpp"
#include "ToyLang.hpp"

class Lexer {
  public:
    Lexer(const std::string &source) : source(source) {}

    std::vector<Token> scanTokens();

  private:
    const std::string source;
    std::vector<Token> tokens;
    std::map<std::string, TokenType> keywords = {
        {"and", TokenType::AND},       {"class", TokenType::CLASS},
        {"else", TokenType::ELSE},     {"false", TokenType::FALSE},
        {"for", TokenType::FOR},       {"fun", TokenType::FUN},
        {"if", TokenType::IF},         {"null", TokenType::NIL},
        {"or", TokenType::OR},         {"print", TokenType::PRINT},
        {"return", TokenType::RETURN}, {"super", TokenType::SUPER},
        {"this", TokenType::THIS},     {"true", TokenType::TRUE},
        {"var", TokenType::VAR},       {"while", TokenType::WHILE},
    };

    int start = 0;
    int current = 0;
    int line = 1;

    void scanToken();

    bool isAtEnd() const;
    char advance();
    char peek() const;
    char peekNext() const;
    bool match(char expected);
    void string();
    void number();
    void identifier();
    void blockComment();
    bool isDigit(char c) const;
    bool isAlpha(char c) const;
    bool isAlphaNumeric(char c) const;
};

#endif