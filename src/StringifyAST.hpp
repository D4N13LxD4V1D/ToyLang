#ifndef TOYLANG_STRINGIFYAST_HPP
#define TOYLANG_STRINGIFYAST_HPP

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Expr.hpp"

class StringifyAST : public ExprVisitor<std::string> {
  public:
    std::string toString(std::unique_ptr<Expr<std::string>> &expr);

    std::string visit(BinaryExpr<std::string> &expr) override;
    std::string visit(GroupingExpr<std::string> &expr) override;
    std::string visit(LiteralExpr<std::string> &expr) override;
    std::string visit(UnaryExpr<std::string> &expr) override;

  private:
    std::string
    parenthesize(const std::string &name,
                 std::vector<std::unique_ptr<Expr<std::string>>> &exprs);
};

#endif