#include "Lexer.hpp"

std::vector<Token> Lexer::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }

    tokens.push_back(
        Token(TokenType::END_OF_FILE, std::string(""), current, 0, line));
    return tokens;
}

void Lexer::scanToken() {
    char c = advance();
    switch (c) {
    case '(':
        tokens.push_back(
            Token(TokenType::LEFT_PAREN, std::string("("), current, 1, line));
        break;
    case ')':
        tokens.push_back(
            Token(TokenType::RIGHT_PAREN, std::string(")"), current, 1, line));
        break;
    case '{':
        tokens.push_back(
            Token(TokenType::LEFT_BRACE, std::string("{"), current, 1, line));
        break;
    case '}':
        tokens.push_back(
            Token(TokenType::RIGHT_BRACE, std::string("}"), current, 1, line));
        break;
    case ',':
        tokens.push_back(
            Token(TokenType::COMMA, std::string(","), current, 1, line));
        break;
    case '.':
        tokens.push_back(
            Token(TokenType::DOT, std::string("."), current, 1, line));
        break;
    case '-':
        tokens.push_back(
            Token(TokenType::MINUS, std::string("-"), current, 1, line));
        break;
    case '+':
        tokens.push_back(
            Token(TokenType::PLUS, std::string("+"), current, 1, line));
        break;
    case ';':
        tokens.push_back(
            Token(TokenType::SEMICOLON, std::string(";"), current, 1, line));
        break;
    case '*':
        tokens.push_back(
            Token(TokenType::STAR, std::string("*"), current, 1, line));
        break;
    case '!':
        if (match('=')) {
            tokens.push_back(Token(TokenType::BANG_EQUAL, std::string("!="),
                                   current, 2, line));
        } else {
            tokens.push_back(
                Token(TokenType::BANG, std::string("!"), current, 1, line));
        }
        break;
    case '=':
        if (match('=')) {
            tokens.push_back(Token(TokenType::EQUAL_EQUAL, std::string("=="),
                                   current, 2, line));
        } else {
            tokens.push_back(
                Token(TokenType::EQUAL, std::string("="), current, 1, line));
        }
        break;
    case '<':
        if (match('=')) {
            tokens.push_back(Token(TokenType::LESS_EQUAL, std::string("<="),
                                   current, 2, line));
        } else {
            tokens.push_back(
                Token(TokenType::LESS, std::string("<"), current, 1, line));
        }
        break;
    case '>':
        if (match('=')) {
            tokens.push_back(Token(TokenType::GREATER_EQUAL, std::string(">="),
                                   current, 2, line));
        } else {
            tokens.push_back(
                Token(TokenType::GREATER, std::string(">"), current, 1, line));
        }
        break;
    case '/':
        if (match('/')) {
            while (!isAtEnd() && peek() != '\n')
                advance();
        } else if (match('*')) {
            blockComment();
        } else {
            tokens.push_back(
                Token(TokenType::SLASH, std::string("/"), current, 1, line));
        }
        break;
    case '#':
        while (!isAtEnd() && peek() != '\n')
            advance();
        break;
    case ' ':
    case '\r':
    case '\t':
        break;
    case '\n':
        line++;
        break;
    case '"':
        string();
        break;
    default:
        if (isDigit(c))
            number();
        else if (isAlpha(c))
            identifier();
        else
            ToyLang::error(current, "Unexpected character.");
        break;
    }
}

bool Lexer::isAtEnd() const { return current >= source.length(); }

char Lexer::advance() { return source[current++]; }

char Lexer::peek() const {
    if (isAtEnd())
        return '\0';

    return source[current];
}

char Lexer::peekNext() const {
    if (isAtEnd())
        return '\0';
    return source[current + 1];
}

bool Lexer::match(char expected) {
    if (isAtEnd())
        return false;
    if (source[current] != expected)
        return false;

    current++;
    return true;
}

void Lexer::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n')
            line++;
        advance();
    }

    if (isAtEnd()) {
        ToyLang::error(line, "Unterminated string");
        return;
    }

    advance(); // closing "

    std::string value = source.substr(start + 1, current - start - 2);

    tokens.push_back(
        Token(TokenType::STRING, value, current, current - start, line));
}

void Lexer::number() {
    while (isDigit(peek()))
        advance();

    if (peek() == '.' && isDigit(peekNext())) {
        advance();

        while (isDigit(peek()))
            advance();
    }

    std::string value = source.substr(start, current - start);
    tokens.push_back(
        Token(TokenType::NUMBER, value, current, current - start, line));
}

void Lexer::identifier() {
    while (isAlphaNumeric(peek()))
        advance();

    std::string text = source.substr(start, current - start);

    TokenType type = TokenType::IDENTIFIER;
    if (keywords.find(text) != keywords.end())
        type = keywords[text];

    tokens.push_back(Token(type, text, current, current - start, line));
}

void Lexer::blockComment() {
    while (!isAtEnd() && peek() != '*' && peekNext() != '/') {
        if (peek() == '\n')
            line++;
        advance();
    }

    if (isAtEnd()) {
        ToyLang::error(line, "Unterminated comment");
        return;
    }

    advance(); // closing *
    advance(); // closing /
}

bool Lexer::isDigit(char c) const { return c >= '0' && c <= '9'; }

bool Lexer::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isAlphaNumeric(char c) const { return isAlpha(c) || isDigit(c); }