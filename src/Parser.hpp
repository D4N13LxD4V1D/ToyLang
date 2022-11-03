#ifndef TOYLANG_PARSER_HPP
#define TOYLANG_PARSER_HPP

#include "Expr.hpp"
#include "Token.hpp"
#include "ToyLang.hpp"

class ParseError : public std::exception {
  public:
    ParseError() {}
};

template <typename T> class Parser {
  public:
    Parser(std::vector<Token> tokens) : tokens(tokens) {}

    std::unique_ptr<Expr<T>> parse();

  private:
    std::vector<Token> tokens;
    int current = 0;

    std::unique_ptr<Expr<T>> expression() noexcept(false);
    std::unique_ptr<Expr<T>> equality() noexcept(false);
    std::unique_ptr<Expr<T>> comparison() noexcept(false);
    std::unique_ptr<Expr<T>> addition() noexcept(false);
    std::unique_ptr<Expr<T>> multiplication() noexcept(false);
    std::unique_ptr<Expr<T>> unary() noexcept(false);
    std::unique_ptr<Expr<T>> primary() noexcept(false);

    bool match(std::vector<TokenType> types);
    bool check(TokenType type);
    Token advance();
    bool isAtEnd();
    Token peek();
    Token previous();
    Token consume(TokenType type, std::string message);

    void synchronize();

    static ParseError error(Token token, std::string message);
};

#include "Parser.tpp"

#endif