#ifndef TOYLANG_TOKEN_HPP
#define TOYLANG_TOKEN_HPP

#include <map>
#include <memory>
#include <ostream>
#include <string>

enum class TokenType {
    // Single-character tokens.
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,

    // One or two character tokens.
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    // Literals.
    IDENTIFIER,
    STRING,
    NUMBER,

    // Keywords.
    AND,
    CLASS,
    ELSE,
    FALSE,
    FUN,
    FOR,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,

    END_OF_FILE
};

class Literal {
  public:
    virtual ~Literal() = default;

    virtual std::string toString() const = 0;

    virtual TokenType getType() const = 0;
};

class StringLiteral : public Literal {
  public:
    StringLiteral(const std::string &value) : value(value) {}

    std::string toString() const { return value; }

    TokenType getType() const { return TokenType::STRING; }

    std::string value;
};

class NumberLiteral : public Literal {
  public:
    NumberLiteral(double value) : value(value) {}

    std::string toString() const { return std::to_string(value); }

    TokenType getType() const { return TokenType::NUMBER; }

    double value;
};

class BooleanLiteral : public Literal {
  public:
    BooleanLiteral(bool value) : value(value) {}

    std::string toString() const { return value ? "true" : "false"; }

    TokenType getType() const {
        return value ? TokenType::TRUE : TokenType::FALSE;
    }

    bool value;
};

class NullLiteral : public Literal {
  public:
    std::string toString() const { return "null"; }

    TokenType getType() const { return TokenType::NIL; }
};

class Token {
  public:
    Token(TokenType type, const std::string &lexeme, int pos, int len, int line)
        : type(type), lexeme(lexeme), pos(pos), len(len), line(line) {}

    std::string toString() const {
        std::map<TokenType, std::string> tokens = {
            {TokenType::LEFT_PAREN, "LEFT_PAREN"},
            {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
            {TokenType::LEFT_BRACE, "LEFT_BRACE"},
            {TokenType::RIGHT_BRACE, "RIGHT_BRACE"},
            {TokenType::COMMA, "COMMA"},
            {TokenType::DOT, "DOT"},
            {TokenType::MINUS, "MINUS"},
            {TokenType::PLUS, "PLUS"},
            {TokenType::SEMICOLON, "SEMICOLON"},
            {TokenType::SLASH, "SLASH"},
            {TokenType::STAR, "STAR"},
            {TokenType::BANG, "BANG"},
            {TokenType::BANG_EQUAL, "BANG_EQUAL"},
            {TokenType::EQUAL, "EQUAL"},
            {TokenType::EQUAL_EQUAL, "EQUAL_EQUAL"},
            {TokenType::GREATER, "GREATER"},
            {TokenType::GREATER_EQUAL, "GREATER_EQUAL"},
            {TokenType::LESS, "LESS"},
            {TokenType::LESS_EQUAL, "LESS_EQUAL"},
            {TokenType::IDENTIFIER, "IDENTIFIER"},
            {TokenType::STRING, "STRING"},
            {TokenType::NUMBER, "NUMBER"},
            {TokenType::AND, "AND"},
            {TokenType::CLASS, "CLASS"},
            {TokenType::ELSE, "ELSE"},
            {TokenType::FALSE, "FALSE"},
            {TokenType::FUN, "FUN"},
            {TokenType::FOR, "FOR"},
            {TokenType::IF, "IF"},
            {TokenType::NIL, "NIL"},
            {TokenType::OR, "OR"},
            {TokenType::PRINT, "PRINT"},
            {TokenType::RETURN, "RETURN"},
            {TokenType::SUPER, "SUPER"},
            {TokenType::THIS, "THIS"},
            {TokenType::TRUE, "TRUE"},
            {TokenType::VAR, "VAR"},
            {TokenType::WHILE, "WHILE"},
            {TokenType::END_OF_FILE, "END_OF_FILE"}};
        return tokens[type] + " " + lexeme;
    }

    TokenType type;
    std::string lexeme;
    int pos;
    int len;
    int line;
};

#endif