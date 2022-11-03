#ifndef TOYLANG_AST_HPP
#define TOYLANG_AST_HPP

#include <memory>
#include <string>
#include <vector>

#include "Token.hpp"

template <typename T> class ExprVisitor;

template <typename T> class Expr {
  public:
    virtual ~Expr() = default;
    virtual T accept(class ExprVisitor<T> &visitor) = 0;
};

template <typename T> class BinaryExpr : public Expr<T> {
  public:
    BinaryExpr(std::unique_ptr<Expr<T>> left, Token op,
               std::unique_ptr<Expr<T>> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}

    virtual T accept(ExprVisitor<T> &visitor) { return visitor.visit(*this); }

    std::unique_ptr<Expr<T>> left;
    Token op;
    std::unique_ptr<Expr<T>> right;
};

template <typename T> class GroupingExpr : public Expr<T> {
  public:
    GroupingExpr(std::unique_ptr<Expr<T>> expression)
        : expression(std::move(expression)) {}

    virtual T accept(ExprVisitor<T> &visitor) { return visitor.visit(*this); }

    std::unique_ptr<Expr<T>> expression;
};

template <typename T> class LiteralExpr : public Expr<T> {
  public:
    LiteralExpr(Token value) : value(value) {}

    virtual T accept(ExprVisitor<T> &visitor) { return visitor.visit(*this); }

    Token value;
};

template <typename T> class UnaryExpr : public Expr<T> {
  public:
    UnaryExpr(Token op, std::unique_ptr<Expr<T>> right)
        : op(op), right(std::move(right)) {}

    virtual T accept(ExprVisitor<T> &visitor) { return visitor.visit(*this); }

    Token op;
    std::unique_ptr<Expr<T>> right;
};

template <typename T> class ExprVisitor {
  public:
    virtual T visit(BinaryExpr<T> &expr) = 0;
    virtual T visit(GroupingExpr<T> &expr) = 0;
    virtual T visit(LiteralExpr<T> &expr) = 0;
    virtual T visit(UnaryExpr<T> &expr) = 0;
};

#endif