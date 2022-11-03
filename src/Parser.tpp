#include "Parser.hpp"

template <typename T> std::unique_ptr<Expr<T>> Parser<T>::parse() {
    try {
        return expression();
    } catch (ParseError &error) {
        return nullptr;
    }
}

template <typename T>
std::unique_ptr<Expr<T>> Parser<T>::expression() noexcept(false) {
    return equality();
}

template <typename T>
std::unique_ptr<Expr<T>> Parser<T>::equality() noexcept(false) {
    std::unique_ptr<Expr<T>> expr = comparison();

    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        Token op = previous();
        std::unique_ptr<Expr<T>> right = comparison();
        expr = std::make_unique<BinaryExpr<T>>(std::move(expr), op,
                                               std::move(right));
    }

    return expr;
}

template <typename T>
std::unique_ptr<Expr<T>> Parser<T>::comparison() noexcept(false) {
    std::unique_ptr<Expr<T>> expr = addition();

    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS,
                  TokenType::LESS_EQUAL})) {
        Token op = previous();
        std::unique_ptr<Expr<T>> right = addition();
        expr = std::make_unique<BinaryExpr<T>>(std::move(expr), op,
                                               std::move(right));
    }

    return expr;
}

template <typename T>
std::unique_ptr<Expr<T>> Parser<T>::addition() noexcept(false) {
    std::unique_ptr<Expr<T>> expr = multiplication();

    while (match({TokenType::MINUS, TokenType::PLUS})) {
        Token op = previous();
        std::unique_ptr<Expr<T>> right = multiplication();
        expr = std::make_unique<BinaryExpr<T>>(std::move(expr), op,
                                               std::move(right));
    }

    return expr;
}

template <typename T>
std::unique_ptr<Expr<T>> Parser<T>::multiplication() noexcept(false) {
    std::unique_ptr<Expr<T>> expr = unary();

    while (match({TokenType::SLASH, TokenType::STAR})) {
        Token op = previous();
        std::unique_ptr<Expr<T>> right = unary();
        expr = std::make_unique<BinaryExpr<T>>(std::move(expr), op,
                                               std::move(right));
    }

    return expr;
}

template <typename T>
std::unique_ptr<Expr<T>> Parser<T>::unary() noexcept(false) {
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token op = previous();
        std::unique_ptr<Expr<T>> right = unary();
        return std::make_unique<UnaryExpr<T>>(op, std::move(right));
    }

    return primary();
}

template <typename T>
std::unique_ptr<Expr<T>> Parser<T>::primary() noexcept(false) {
    if (match({TokenType::FALSE, TokenType::TRUE, TokenType::NIL,
               TokenType::NUMBER, TokenType::STRING})) {
        return std::make_unique<LiteralExpr<T>>(previous());
    }

    if (match({TokenType::LEFT_PAREN})) {
        std::unique_ptr<Expr<T>> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<GroupingExpr<T>>(std::move(expr));
    }

    std::cout << "Error: " << peek().toString() << std::endl;

    throw error(peek(), "Expect expression.");
}

template <typename T> bool Parser<T>::match(std::vector<TokenType> types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }

    return false;
}

template <typename T> bool Parser<T>::check(TokenType type) {
    if (isAtEnd())
        return false;
    return peek().type == type;
}

template <typename T> Token Parser<T>::advance() {
    if (!isAtEnd())
        current++;
    return previous();
}

template <typename T> bool Parser<T>::isAtEnd() {
    return peek().type == TokenType::END_OF_FILE;
}

template <typename T> Token Parser<T>::peek() { return tokens[current]; }

template <typename T> Token Parser<T>::previous() {
    return tokens[current - 1];
}

template <typename T>
Token Parser<T>::consume(TokenType type, std::string message) {
    if (check(type))
        return advance();

    throw error(peek(), message);
}

template <typename T> void Parser<T>::synchronize() {
    advance();

    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON)
            return;

        switch (peek().type) {
        case TokenType::CLASS:
        case TokenType::FUN:
        case TokenType::VAR:
        case TokenType::FOR:
        case TokenType::IF:
        case TokenType::WHILE:
        case TokenType::PRINT:
        case TokenType::RETURN:
            return;
        default:
            break;
        }

        advance();
    }
}

template <typename T>
ParseError Parser<T>::error(Token token, std::string message) {
    if (token.type == TokenType::END_OF_FILE)
        Logger::report(token.line, " at end", message);
    else
        Logger::report(token.line, " at '" + token.lexeme + "'", message);
    return ParseError();
}